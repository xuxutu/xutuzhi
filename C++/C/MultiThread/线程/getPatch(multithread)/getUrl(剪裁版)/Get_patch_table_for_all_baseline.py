#!/usr/bin/env python 
# -*- coding:utf-8 -*-
import argparse
import json
import os
import random
import re

from pyvirtualdisplay import Display
from selenium import webdriver
from selenium.webdriver import DesiredCapabilities
from selenium.webdriver.common.keys import Keys


# global parameter define
import time

qcom_url = None
qcom_user = None
qcom_pass = None
DEBUG = False
custome_webdriver = None
download_path = None
download_filename = None

# user_agent = [
#     "Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10_6_8; en-us) AppleWebKit/534.50 (KHTML, like Gecko) Version/5.1 Safari/534.50",
#     "Mozilla/5.0 (Windows; U; Windows NT 6.1; en-us) AppleWebKit/534.50 (KHTML, like Gecko) Version/5.1 Safari/534.50",
#     "Mozilla/5.0 (Windows NT 10.0; WOW64; rv:38.0) Gecko/20100101 Firefox/38.0",
#     "Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; .NET4.0C; .NET4.0E; .NET CLR 2.0.50727; .NET CLR 3.0.30729; .NET CLR 3.5.30729; InfoPath.3; rv:11.0) like Gecko",
#     "Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0)",
#     "Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.0; Trident/4.0)",
#     "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 6.0)",
#     "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)",
#     "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.6; rv:2.0.1) Gecko/20100101 Firefox/4.0.1",
#     "Mozilla/5.0 (Windows NT 6.1; rv:2.0.1) Gecko/20100101 Firefox/4.0.1",
#     "Opera/9.80 (Macintosh; Intel Mac OS X 10.6.8; U; en) Presto/2.8.131 Version/11.11",
#     "Opera/9.80 (Windows NT 6.1; U; en) Presto/2.8.131 Version/11.11",
#     "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_0) AppleWebKit/535.11 (KHTML, like Gecko) Chrome/17.0.963.56 Safari/535.11",
#     "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Maxthon 2.0)",
#     "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; TencentTraveler 4.0)",
#     "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)",
#     "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; The World)",
#     "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Trident/4.0; SE 2.X MetaSr 1.0; SE 2.X MetaSr 1.0; .NET CLR 2.0.50727; SE 2.X MetaSr 1.0)",
#     "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; 360SE)",
#     "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Avant Browser)",
#     "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)",
#     "Mozilla/5.0 (iPhone; U; CPU iPhone OS 4_3_3 like Mac OS X; en-us) AppleWebKit/533.17.9 (KHTML, like Gecko) Version/5.0.2 Mobile/8J2 Safari/6533.18.5",
#     "Mozilla/5.0 (iPod; U; CPU iPhone OS 4_3_3 like Mac OS X; en-us) AppleWebKit/533.17.9 (KHTML, like Gecko) Version/5.0.2 Mobile/8J2 Safari/6533.18.5",
#     "Mozilla/5.0 (iPad; U; CPU OS 4_3_3 like Mac OS X; en-us) AppleWebKit/533.17.9 (KHTML, like Gecko) Version/5.0.2 Mobile/8J2 Safari/6533.18.5",
#     "Mozilla/5.0 (Linux; U; Android 2.3.7; en-us; Nexus One Build/FRF91) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1",
#     "MQQBrowser/26 Mozilla/5.0 (Linux; U; Android 2.3.7; zh-cn; MB200 Build/GRJ22; CyanogenMod-7) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1",
#     "Opera/9.80 (Android 2.3.4; Linux; Opera Mobi/build-1107180945; U; en-GB) Presto/2.8.149 Version/11.10",
#     "Mozilla/5.0 (Linux; U; Android 3.0; en-us; Xoom Build/HRI39) AppleWebKit/534.13 (KHTML, like Gecko) Version/4.0 Safari/534.13",
#     "Mozilla/5.0 (BlackBerry; U; BlackBerry 9800; en) AppleWebKit/534.1+ (KHTML, like Gecko) Version/6.0.0.337 Mobile Safari/534.1+",
#     "Mozilla/5.0 (hp-tablet; Linux; hpwOS/3.0.0; U; en-US) AppleWebKit/534.6 (KHTML, like Gecko) wOSBrowser/233.70 Safari/534.6 TouchPad/1.0",
#     "Mozilla/5.0 (SymbianOS/9.4; Series60/5.0 NokiaN97-1/20.0.019; Profile/MIDP-2.1 Configuration/CLDC-1.1) AppleWebKit/525 (KHTML, like Gecko) BrowserNG/7.1.18124",
#     "Mozilla/5.0 (compatible; MSIE 9.0; Windows Phone OS 7.5; Trident/5.0; IEMobile/9.0; HTC; Titan)",
#     "UCWEB7.0.2.37/28/999",
#     "NOKIA5700/ UCWEB7.0.2.37/28/999",
#     "Openwave/ UCWEB7.0.2.37/28/999",
#     "Mozilla/4.0 (compatible; MSIE 6.0; ) Opera/UCWEB7.0.2.37/28/999",
#     # iPhone 6：
#     "Mozilla/6.0 (iPhone; CPU iPhone OS 8_0 like Mac OS X) AppleWebKit/536.26 (KHTML, like Gecko) Version/8.0 Mobile/10A5376e Safari/8536.25",
#
# ]
#
# headers = {'User-Agent': random.choice(user_agent)}


def save_to_file(content):
    fp = open("DLfile.conf", "a+")
    fp.write(content)
    fp.close()


def load_config(confile):
    return json.load(open(confile))


def init_webdriver(driver_path, options, retry=1):
    print("[retry = {}] on init_webdriver".format(retry))
    try:
        # webdriver.Chrome(executable_path="/usr/local/share/chromedriver",chrome_options=options).set_page_load_timeout(20)
        return webdriver.Chrome(executable_path="/usr/local/share/chromedriver", chrome_options=options)
    except Exception as e:
        print(e)
        if retry == 0:
            print("Error when use webdriver.Chrome : ".format(e.message))
            return None
        return init_webdriver(driver_path, options, retry - 1)


def web_login(func):
    def _web_login(*args, **kwargs):
        display = Display(visible=0, size=(800, 600))  # TODO: no display
        display.start()
        # options = chrome_option()
        options = webdriver.ChromeOptions()
        options.add_experimental_option("prefs", {
            "download.prompt_for_download": False,
            "download.directory_upgrade": True,
            "safebrowsing.enabled": True,
            "download.default_directory": download_path,
        })
        

	#options.add_argument("--no-sandbox")
        options.add_argument("--disable-setuid-sandbox")
        options.add_argument('--disable-infobars')
        # options.set_headless()
        options.add_argument('--disable-dev-shm-usage')
        # options.add_argument('--headless')
        options.add_argument('blink-settings=imagesEnabled=false')
        options.add_argument('--disable-gpu')
        # options.add_argument(
        #    'User-Agent="Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.122 Safari/537.36"')

        print(options.to_capabilities())

        # desired_capabilities = DesiredCapabilities.CHROME
        # desired_capabilities["pageLoadStrategy"] = "none"

        global browser
        browser = init_webdriver(custome_webdriver if custome_webdriver else "chromedriver", options, retry=3)
        browser.implicitly_wait(30)
        # browser.set_page_load_timeout(30)
        browser.get(qcom_url)

        if DEBUG:
            print("before current url:" + str(browser.current_url))  # for debug

        # 2019.09.05   network problem,so change 10->15
        time.sleep(15)  # TODO: FIX wait loading element

        user = browser.find_element_by_css_selector("#email")
        try:
            user.clear()
            user.send_keys(qcom_user)
        except Exception as e:
            print(e)
            print("find user input failed!")

        if DEBUG:
            print("User input ok")
            print("user ok  current url:" + str(browser.current_url))

        passwd = browser.find_element_by_css_selector("#PASSWORD_INPUT")

        try:
            passwd.clear()
            passwd.send_keys(qcom_pass + Keys.ENTER)
            print("passwd ok  current url:" + str(browser.current_url))
        except Exception as e:
            print(e)
            print("find passwd input failed!")

        if DEBUG:
            print("password input ok")
            print("click ok")

        time.sleep(10)
        if DEBUG:
            print("after current url:" + str(browser.current_url))

        print("[web_login]login ok current url: " + str(browser.current_url))

        ret = func(*args, **kwargs)
        browser.close()
        display.stop()
        return ret

    return _web_login


@web_login
def download_cr_info_file(cr_nums):
    mybaseline = "MSM8998.LA.2.0.2"
    print("Total CR number : " + str(len(cr_nums)))

    for i in range(len(cr_nums)):
        print("cr number :" + cr_nums[i])

        # print("[download_cr_info_file]login ok current url: " + str(browser.current_url))

        realURL = "https://createpoint.qti.qualcomm.com/planner/#search/newReleaseHistory/searchArgs/rows||20||sortField||name||sortOrder||asc||isSpf||false||releaseName||" + \
                  mybaseline + "||branch||master||product||" + mybaseline + "||fq_facetCrStatus||Complete~~InProgress||fq_facetCrNumber||" + cr_nums[i]

        print("[NOTE]You can use the following URL to find the patch manually:\n" + realURL)
        browser.get(realURL)
        time.sleep(8)

        if DEBUG:
            print("[download_cr_info_file] current_url: " + str(browser.current_url))
        # download-all-crfix
        browser.find_element_by_css_selector("#download-all-crfix").click()


        time.sleep(8)
        print(os.listdir(download_path))

        excelname_list = os.listdir(download_path)
        for excelname in excelname_list:
            if "ReleaseHistory.xls" == excelname:
                new_name = excelname.replace(excelname, "ReleaseHistory_%s.xls" % str(cr_nums[i]))
                os.rename(os.path.join(download_path, excelname), os.path.join(download_path, new_name))
                print("The download file is " + str(new_name))
                print(">>>>>>>The file " + str(new_name) + " download successfully!<<<<<<")
                save_to_file(str(new_name) + "\n")
        if "ReleaseHistory_%s.xls" % str(cr_nums[i]) not in os.listdir(download_path):
            print("CR number " + cr_nums[i] + " file download failed!!!")


if __name__ == "__main__":
    list = []
    parser = argparse.ArgumentParser(
        prog="Security Toolkit", description='',
        epilog='Copyright © 2008-2020 Thundersoft all rights reserved.')
    parser.add_argument('--no-sandbox')
    parser.add_argument('--cr', action='store', type=str)
    parser.add_argument('--downloadpath', action='store', type=str)
    parser.add_argument('--baseline', action='store', type=str)
    parser.add_argument('--webdriver', action='store',
                        default='{}/chromedriver'.format(
                            os.path.dirname(os.path.abspath(__file__))))
    parser.add_argument('--conf', action='store',
                        default='{}/config.json'.format(
                            os.path.dirname(os.path.abspath(__file__))))
    args = parser.parse_args()
    custome_webdriver = args.webdriver

    conf_obj = load_config(args.conf)
    qcom_url = conf_obj['config']['qcom']['url']
    qcom_user = conf_obj['config']['qcom']['user']
    qcom_pass = conf_obj['config']['qcom']['pass']

    download_path = args.downloadpath if args.downloadpath else "{}".format(os.path.dirname(os.path.abspath(__file__)))
    baseline = args.baseline
    list = str(args.cr).split(' ')

    files_get = []
    download_cr_info_file(list)

    print(">>>>>>>>>>>>>>>>>>>>> ALL CR file download over <<<<<<<<<<<<<<<<<<<<<")


