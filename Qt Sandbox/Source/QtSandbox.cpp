#pragma once

#include <Ascend.hpp>
#include <QtWidgets>
#include <QVulkanInstance.h>

class QtSandbox : public QWindow
{
private:
	static constexpr char APP_NAME[] = "Ascend QT Sandbox";

	const asc::Application* application = nullptr;
	const asc::Renderer* renderer = nullptr;
	bool isInitialized = false;

	std::vector<const char*> queryInstanceExtensions()
	{
		const auto temporaryVulkanInstance = new QVulkanInstance();
		temporaryVulkanInstance->create();
		const auto instanceExtensions = temporaryVulkanInstance->extensions();
		std::vector<const char*> output(instanceExtensions.begin(), instanceExtensions.end());

		std::vector<const char*> output;
		output.resize(instanceExtensions.size());
		for (int i = 0; i < instanceExtensions.size(); ++i)
		{
			/*const auto t = instanceExtensions[i];
			const auto u = t.toStdString();
			const auto v = u.c_str();
			*/

			const auto u = instanceExtensions[i];
			const auto v = u.constData();
			output[i] = v;
		}

		delete temporaryVulkanInstance;
		return output;
	}

	void init()
	{
		auto applicationInfo = asc::ApplicationInfo().setName("Test").setVersion(1, 0, 0).setInstanceExtensions(queryInstanceExtensions());
		for (const auto& i : applicationInfo.instanceExtensions)
			qDebug() << "\"" << i << "\"";

		#ifdef DEBUG
			applicationInfo.debugCallbackLambda = [](const std::string& message)
			{
				QMessageBox::critical(nullptr, "Error", message.c_str());
			};
		#endif

		applicationInfo.createSurfaceLambda = [&](const VkInstance* instance) -> VkSurfaceKHR*
		{
			return new VkSurfaceKHR(QVulkanInstance::surfaceForWindow(this));
		};

		application = new asc::Application(applicationInfo); // TODO: mem leak
		
		const auto vulkanInstance = new QVulkanInstance(); // TODO: mem leak?
		vulkanInstance->setVkInstance(*application->getInstance());
		
		if (!vulkanInstance->create())
		{
			throw std::runtime_error("Error: Failed to initialize Vulkan instance.");
		}

		setVulkanInstance(vulkanInstance);

		renderer = new asc::Renderer(*application); // TODO: mem leak
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

struct MainWindow : public QMainWindow
{
public:
	MainWindow()
	{
		resize(1280, 720);

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