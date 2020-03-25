#include<bits/stdc++.h>
#include "curl/curl.h"
#include "BaseCatcher.h"
#include "BangumiCatcher.h"
#include "IntToString.h"
using namespace std;

BangumiCatcher::BangumiCatcher(): BaseCatcher("Bangumi") {}
void BangumiCatcher::catchWebsite() {
	system(("mkdir " + webName).c_str()); //make a directory for bangumi.tv
	curl_global_init(CURL_GLOBAL_DEFAULT);
	
	saveImage("http://lain.bgm.tv/pic/cover/l/c2/4c/253_t3XWb.jpg", "1.jpg");
	
	saveHTML("http://bangumi.tv/anime/browser?sort=rank", webName + "/page1.html");
	ifstream fin(webName + "/page1.html");
	int maxPageNumber = 0;
	string html;
	const string pattern = "?sort=rank&page="; //get the list of bangumis sorted by points
	while (getline(fin, html)) {
		int pos = 0;
		while (pos < html.length()) {
			pos = html.find(pattern, pos);
			if (pos == string::npos) break;
			pos += pattern.length();
			int pageNumber = 0;
			while (pos < html.length() && html[pos] >= '0' && html[pos] <= '9') {
				pageNumber = pageNumber * 10 + html[pos] - '0';
				pos++;
			}
			maxPageNumber = max(maxPageNumber, pageNumber);
		}
	}
	//cout << maxPageNumber << endl;
	for (int i = 2; i <= maxPageNumber; i++)
		saveHTML("http://bangumi.tv/anime/browser?sort=rank&page=" + IntToString(i), webName + "/page" + IntToString(i) + ".html");
	
	const string subject = "http://bangumi.tv/subject/"; //download all pages
	for (int i = 1; i <= maxPageNumber; i++) {
		ifstream fin(webName + "/page" + IntToString(i) + ".html");
		const string pattern = "/subject/";
		while (getline(fin, html)) {
			int pos = 0;
			while (pos < html.length()) {
				pos = html.find(pattern, pos);
				if (pos == string::npos) break;
				pos += pattern.length();
				string id = "";
				while (pos < html.length() && html[pos] >= '0' && html[pos] <= '9') {
					id.push_back(html[pos]);
					pos++;
				}
				if (saveHTML(subject + id, webName + "/" + id + ".html"))
					insertPage(id + ".html", 1);
			}
		}
	}
	savePage();
	
	curl_global_cleanup();
}
