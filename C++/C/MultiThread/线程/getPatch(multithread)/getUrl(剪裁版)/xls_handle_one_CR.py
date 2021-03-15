#!/usr/bin/env python 
# -*- coding:utf-8 -*-
import datetime
import os
import sys
from openpyxl import load_workbook
import xlrd
import xlwt
import xlutils
import struct
from lxml import etree
from pyvirtualdisplay import Display
from selenium import webdriver
import argparse

import time

custome_webdriver = None
patch_num = 0
url_number = 0

# struct CR_info
# {
#     unsigned int CR;
#     list Related_Files;
#     list Commit_ID;
#     list VCS_Project;
#     string CVE;
# # cloumn[24] Related Files
# # cloumn[25] Commit ID
# # cloumn[26] VCS Project
# }


class CrInfo:
    def __init__(self):
        self.CR = ""
        self.Related_Files = []
        self.Commit_ID = []
        self.VCS_Project = []
        self.CVE = ""


def get_info_from_table(filename):
    workbook = xlrd.open_workbook(filename)
    sheet_name_all = workbook.sheet_names()
    # print(sheet_name_all)

    sheet = workbook.sheet_by_name("ReleaseHistory")
    rows_max = sheet.nrows
    cols_max = sheet.ncols

    ALL_CRs = sheet.col_values(0)
    ALL_CRs.pop(0)
    # print(ALL_CRs)

    ALL_Related_Files = sheet.col_values(25)
    ALL_Related_Files.pop(0)
    print(ALL_Related_Files)

    ALL_Commit_ID = sheet.col_values(26)
    ALL_Commit_ID.pop(0)
    print(ALL_Commit_ID)

    ALL_VCS_Project = sheet.col_values(27)
    ALL_VCS_Project.pop(0)
    print(ALL_VCS_Project)

    ALL_CVE = sheet.col_values(cols_max-1)
    ALL_CVE.pop(0)
    # print(ALL_CVE)

    # patch_info :
    # type: dict
    # key: CR
    # content: Related_Files(list) Commit_ID(list) VCS_Project(list)  CVE(list)
    patch_info = {}
    for i in range(rows_max-1):
        patch_info[ALL_CRs[i]] = [ALL_Related_Files[i], ALL_Commit_ID[i], ALL_VCS_Project[i], ALL_CVE[i]]

    # for CR in ALL_CRs:
    #     print(CR + ": \n")
    #     print(patch_info[CR])
    #     print("\n")

    # cloumn[24] Related Files
    # cloumn[25] Commit ID
    # cloumn[26] VCS Project
    # print("Max row = " + str(rows_max) + "; Max column = " + str(cols_max))

    Info_list = []
    for i in range(rows_max-1):
        Info_list.append(info_handle(ALL_CRs[i],patch_info[ALL_CRs[i]]))

    # print(Info_list[0].VCS_Project)
    return Info_list


def info_handle(CR, dict_value):
    newCrInfo = CrInfo()
    newCrInfo.CR = CR
    newCrInfo.Related_Files = dict_value[0].split("\n")
    newCrInfo.Commit_ID = dict_value[1].split("\n")
    newCrInfo.VCS_Project = dict_value[2].split("\n")
    newCrInfo.CVE = dict_value[3].split("\n")
    return newCrInfo


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


def web_set(func):
    def _web_set(*args, **kwargs):
        display = Display(visible=0, size=(1366, 768))  # TODO: no display
        display.start()
        # options = chrome_option()
        options = webdriver.ChromeOptions()
        options.add_experimental_option("prefs", {
            "download.prompt_for_download": False,
            "download.directory_upgrade": True,
            "safebrowsing.enabled": True,
            "download.default_directory": os.path.dirname(os.path.abspath(__file__))})
        options.add_argument("--no-sandbox")
        options.add_argument("--disable-setuid-sandbox")
        # options.add_argument('lang=zh_CN.UTF-8')
        options.add_argument(
            'User-Agent="Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.122 Safari/537.36"')
        options.add_argument('--headless')
        print(options.to_capabilities())

        global browser
        browser = init_webdriver(custome_webdriver if custome_webdriver else "chromedriver", options, retry=3)
        browser.implicitly_wait(120)
        ret = func(*args, **kwargs)
        browser.close()
        display.stop()
        return ret

    return _web_set


@web_set
def get_and_save_patch(cr_num, commit_list, project_list):
    global Total_url_number
    Total_url_number = len(commit_list) * len(project_list)
    print("Total : " + str(Total_url_number))
    # import current time to rename patch files
    now_time = datetime.datetime.now()
    time_name = now_time.strftime('%m%d_%H%M%S')

    # get the commit url
    commit_url = None
    global patch_num
    patch_num = 0
    global url_number
    url_number = 0
    global browser
    for project in project_list:
        for commit in commit_list:
            try:
                commit_url = "https://source.codeaurora.org/" + project + "/patch/?id=" + commit
                print(commit_url)
                save_to_file("ALL_URL_" + str(cr_num) + ".txt", commit_url + "\n", "a+")
                patch_retry = 3

                #--browser.get(commit_url)

                #--time.sleep(8)  # TODO: FIX wait loading element
                page = browser.page_source
                if "</pre>" in page and "cgit" in page:
                    tree = etree.HTML(page)
                    patch = tree.xpath("//pre/text()")[0]
                    patch_num = patch_num + 1
                    cr_file = cr_num + "_" + str(patch_num) + "_" + commit + ".patch"

                    new_dirname = cr_num + "_" + time_name

                    # create dir
                    new_path = new_dirname + "/" + project
                    os.makedirs(new_path,exist_ok=True)
                    new_file = new_path + "/" + cr_file
                    if os.path.exists(new_file):
                        os.remove(new_file)
                    os.mknod(new_file)

                    # print("cr file name:" + new_file)
                    fp = open(new_file, "w")

                    fp.write(patch)
                    fp.close()
                    print("detected the patch, save to: " + cr_file)
                else:
                    print("no </pre> or cgit")
                url_number += 1
                print(
                    "[Count]No." + str(url_number) + " url, " + str(
                        int(Total_url_number) - int(url_number)) + " url left.")
                # break

            except Exception as e:
                print(e)
                print("[error] URL : " + commit_url)
                fp2 = open("error_URL.txt", "a+")
                fp2.write(commit_url + "\n")
                fp2.close()
                pass
            continue


def save_to_file(filename, content, mode):
    fp = open(filename, mode)
    fp.write(content)
    fp.close()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog="Security Toolkit", description='',
        epilog='Copyright © 2008-2020 Thundersoft all rights reserved.')
    parser.add_argument('--no-sandbox')
    parser.add_argument('--cr', action='store', type=str)
    parser.add_argument('--baseline', action='store', type=str)
    parser.add_argument('--filename', action='store', type=str)
    parser.add_argument('--webdriver', action='store',
                        default='{}/chromedriver'.format(
                            os.path.dirname(os.path.abspath(__file__))))
    parser.add_argument('--conf', action='store',
                        default='{}/config.json'.format(
                            os.path.dirname(os.path.abspath(__file__))))
    args = parser.parse_args()
    custome_webdriver = args.webdriver

    filename = args.filename
    list = str(args.cr).split(' ')

    patch_InFo_list = []
    for i in range(len(list)):
        patch_InFo_list = get_info_from_table("ReleaseHistory_%s.xls" % str(list[i]))
        print("[get_info_from_table] ok")
        print("[CR COUNT]No." + str(i+1) + " CR, " + str(len(list)-i) + " left.")
        if not patch_InFo_list:
            print("CR number " + list[i] + " has NOT been released for all baseline!!!")
            continue
        else:
            patch_info = patch_InFo_list[0]
            Related_Files = "\n".join(patch_info.Related_Files)
            Commit_ID = "\n".join(patch_info.Commit_ID)
            VCS_Project = "\n".join(patch_info.VCS_Project)
            CVE = "\n".join(patch_info.CVE)
            content = "Related Files:\n" + Related_Files + "\n\nCommit ID:\n" + Commit_ID + \
                "\n\nVCS Project:\n" + VCS_Project + "\n\nCVE:\n" + CVE
        # if Related_Files == "" and Commit_ID == "" and VCS_Project == "" and CVE == "":
        #     print("CR number " + list[i] + " has NOT been released for all baseline!!!")
        #     continue
            save_to_file("patch_info_" + str(patch_info.CR) + ".txt", content, "w")
            get_and_save_patch(patch_info.CR, patch_info.Commit_ID, patch_info.VCS_Project)
            print("CR " + list[i] + " patch get ok!")




