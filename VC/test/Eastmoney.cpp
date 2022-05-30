#include "environment.h"
#include <webdriverxx/browsers/chrome.h>
#include <webdriverxx/browsers/firefox.h>
#include <webdriverxx/browsers/ie.h>
#include <webdriverxx/webdriver.h>
#include <gtest/gtest.h>
#include <string>
#include <process.h>
#include <string.h>

#include <chrono>
#include <thread>
#include <pystring/pystring.h>
#include <gtest/gtest.h>

namespace test {

	using namespace webdriverxx;
	Environment* Environment::instance_ = 0;

	bool IsCommandLineArgument(const std::string& arg, const char* name) {
		return arg.find(std::string("--") + name) == 0;
	}

	std::string GetCommandLineArgumentValue(const std::string& arg) {
		const size_t pos = arg.find('=');
		return pos == std::string::npos ? std::string() : arg.substr(pos + 1);
	}

	Parameters ParseParameters(int argc, char** argv) {
		Parameters result;
		for (int i = 1; i < argc; ++i) {
			const std::string arg = argv[i];
			if (IsCommandLineArgument(arg, "browser")) {
				result.web_driver_url = webdriverxx::kDefaultWebDriverUrl;
				const std::string browser_name = GetCommandLineArgumentValue(arg);
				result.desired.Set("browserName", browser_name);
			}
			else if (IsCommandLineArgument(arg, "pages")) {
				result.test_pages_url = GetCommandLineArgumentValue(arg);
			}
			else if (IsCommandLineArgument(arg, "webdriver")) {
				result.web_driver_url = GetCommandLineArgumentValue(arg);
			}
			else if (IsCommandLineArgument(arg, "test_real_browsers")) {
				result.test_real_browsers = true;
			}
		}
		return result;
	}

} // namespace test

using namespace webdriverxx;
WebDriver initWebDriver() {
	auto caps = Capabilities();
	caps.SetVersion("93.0.4577.63");
	caps.SetBrowserName(browser::Chrome);
	caps.SetPlatform(platform::Any);
	WebDriver driver = Start(Chrome(caps));
	return std::move(driver);
}

GTEST_TEST(WdTests, TestBaidu) {
	auto driver = initWebDriver();
	driver.Navigate("https://www.baidu.com");
	auto kw = driver.FindElement(ById("kw"));
	kw.SendKeys("Hello");
	auto su = driver.FindElement(ById("su"));
	su.Click();
	std::this_thread::sleep_for(std::chrono::seconds(1));
	auto source = driver.GetPageSource();
	auto source_lower = pystring::lower(source);
	ASSERT_TRUE(source_lower.find("hello") != std::string::npos);
}

int main(int argc, char** argv) {
	bool x = true;
	::testing::InitGoogleTest(&argc, argv);
	::testing::AddGlobalTestEnvironment(
		new test::Environment(test::ParseParameters(argc, argv))
	);
	x = false;
	if (x)
	{

	}
	//
	return RUN_ALL_TESTS();
}