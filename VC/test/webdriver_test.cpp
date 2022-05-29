#include "environment.h"
#include <webdriverxx/webdriver.h>
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
	TEST(WebDriver, AndSatelliteObjectsHasNoLifetimeIssues);
	return 0;
}
namespace test {

using namespace webdriverxx;

TEST(WebDriver, CreatesSession) {
	Client client(GetWebDriverUrl());
	size_t number_of_sessions_before = client.GetSessions().size();
	WebDriver testee = CreateDriver();
	size_t number_of_sessions_after = client.GetSessions().size();
	ASSERT_EQ(number_of_sessions_before + 1, number_of_sessions_after);
}

TEST(WebDriver, DeletesSessionOnDestruction) {
	Client client(GetWebDriverUrl());
	size_t number_of_sessions_before = 0;
	{
		WebDriver testee = CreateDriver();
		number_of_sessions_before = client.GetSessions().size();
	}
	size_t number_of_sessions_after = client.GetSessions().size();
	ASSERT_EQ(number_of_sessions_before - 1, number_of_sessions_after);
}

TEST(WebDriver, IsCopyable) {
	WebDriver driver1(GetDriver());
	const WebDriver driver2 = driver1;
	WebDriver driver3 = driver1;
	driver3 = driver2;
	ASSERT_NO_THROW(GetDriver().GetSessions());
	ASSERT_NO_THROW(driver1.GetSessions());
	ASSERT_NO_THROW(driver2.GetSessions());
	ASSERT_NO_THROW(driver3.GetSessions());
}

TEST(WebDriver, CopyableToClient) {
	WebDriver driver = GetDriver();
	Client client = driver;
	ASSERT_NO_THROW(client.GetSessions());
	ASSERT_NO_THROW(driver.GetSessions());
}

TEST(WebDriver, CopyableToSession) {
	WebDriver driver = GetDriver();
	Session session = driver;
	ASSERT_NO_THROW(session.GetWindows());
	ASSERT_NO_THROW(driver.GetWindows());
}

TEST(WebDriver, AndSatelliteObjectsHasNoLifetimeIssues) {
	// It is too expensive to restart the driver ->
	// try to test all objects in one test.
	WebDriver& driver = GetDriver();
	driver.Navigate(GetTestPageUrl("webdriver.html"));

	Element body = driver.FindElement(ByTag("body"));
	{
		Window window = driver.GetCurrentWindow();
		{
			Session session = driver;
			{
				Client client = driver;
				{
					WebDriver local_driver = driver;
					GetFreshDriver(); // Destroy global instance
					ASSERT_NO_THROW(local_driver.GetSessions());
					ASSERT_NO_THROW(local_driver.GetWindows());
					ASSERT_NO_THROW(local_driver.FindElement(ByTag("input")));
				}
				ASSERT_NO_THROW(client.GetSessions());
			}
			ASSERT_NO_THROW(session.GetWindows());
			ASSERT_NO_THROW(session.FindElement(ByTag("input")));
		}
		ASSERT_NO_THROW(window.GetSize());
	}
	ASSERT_NO_THROW(body.FindElement(ByTag("input")));
}

} // namespace test
