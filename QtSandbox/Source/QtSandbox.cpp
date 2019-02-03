#pragma once

#include <Ascend.hpp>
#include <QtWidgets>
#include <QVulkanInstance.h>

class QtSandbox : public QWindow
{
private:
	static constexpr char APP_NAME[] = "Ascend QT Sandbox";

	std::unique_ptr<asc::Application> application;
	std::unique_ptr<asc::Renderer> renderer;
	std::unique_ptr<QVulkanInstance> vulkanInstance;
	bool isInitialized = false;

	std::vector<const char*> queryInstanceExtensions(QVulkanInstance& temporaryVulkanInstance)
	{
		const auto instanceExtensions = temporaryVulkanInstance.extensions();
		return std::vector<const char*>(instanceExtensions.begin(), instanceExtensions.end());
	}

	void init()
	{
		auto temporaryVulkanInstance = QVulkanInstance();
		temporaryVulkanInstance.create();
		auto applicationInfo = asc::ApplicationInfo().setName(APP_NAME).setVersion(1, 0, 0).setInstanceExtensions(queryInstanceExtensions(temporaryVulkanInstance));

		#ifdef DEBUG
			applicationInfo.debugCallbackLambda = [](const std::string& message)
			{
				qDebug() << message.c_str();
				QMessageBox::critical(nullptr, "Error", message.c_str());
			};
		#endif

		applicationInfo.createSurfaceLambda = [&](const VkInstance* instance) -> VkSurfaceKHR*
		{
			return new VkSurfaceKHR(QVulkanInstance::surfaceForWindow(this));
		};

		application = std::make_unique<asc::Application>(applicationInfo);
		
		vulkanInstance = std::make_unique<QVulkanInstance>();
		vulkanInstance->setVkInstance(*application->getInstance());
		
		if (!vulkanInstance->create())
		{
			throw std::runtime_error("Error: Failed to initialize Vulkan instance.");
		}

		setVulkanInstance(vulkanInstance.get());

		renderer = std::make_unique<asc::Renderer>(*application);
	}

	void exposeEvent(QExposeEvent*) override
	{
		if (isExposed() && !isInitialized)
		{
			init();
			renderer->renderFrame();
			isInitialized = true;
		}
	}

public:
	QtSandbox()
	{
		setSurfaceType(QSurface::VulkanSurface);
	}
};

class MainWindow : public QMainWindow
{
private:
	static constexpr auto WINDOW_WIDTH = 1280;
	static constexpr auto WINDOW_HEIGHT = 720;

public:
	MainWindow()
	{
		resize(WINDOW_WIDTH, WINDOW_HEIGHT);

		const auto rootLayout = new QHBoxLayout();
		const auto rootWidget = new QWidget();
		rootWidget->setLayout(rootLayout);
		rootLayout->addWidget(QWidget::createWindowContainer(new QtSandbox()));
		rootLayout->addWidget(new QLabel("Welcome to the Ascend Qt Sandbox application."));
		setCentralWidget(rootWidget);
	}
};

struct MyApplication : public QApplication
{
	MyApplication::MyApplication(int& argc, char** argv) : QApplication(argc, argv) { }

	bool MyApplication::notify(QObject* receiver, QEvent* event)
	{
		bool done = true;

		try
		{
			done = QApplication::notify(receiver, event);
		}
		catch (const std::exception& error)
		{
			QMessageBox::critical(nullptr, "Error", error.what());
		}
	
		return done;
	}
};

int main(int argc, char *argv[])
{
	MyApplication a(argc, argv);
	MainWindow mainWindow;
	mainWindow.show();
	return a.exec();
}