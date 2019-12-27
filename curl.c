#include "main.h"

string findHREF(char *fname) {
	FILE *fp;
	int line_num = 1;
	int find_result = 0;
	char temp[512];

	if((fp = fopen(fname, "r")) == NULL) {
		return(-1);
	}

	while(fgets(temp, 512, fp) != NULL) {
		if((strstr(temp, "href=")) != NULL) {
            char* pt = (strstr(temp, "href=")) + 6;
            char* pt2 = (strstr(pt, '"');
            int len = pt2 - pt;
            char* newURL = malloc(sizeof(char)*len);
            strncpy(newURL, p1, len);
            newURL[len] = '\0';
            return newURL;


		}
		line_num++;
	}


	//Close the file if still open.
	if(fp) {
		fclose(fp);
	}
   	return NULL;
}

void getPage(string url, const char* fileName){
  CURL* curl;
  CURLcode res;
  FILE* file;
  
  file = fopen(fileName, "w");
  
  if(file == NULL){
	fprintf("Error while writing %s, in %s at line #%d\n",fileName, __FILE__, __LINE__);
	fclose(file);
	exit(EXIT_FAILURE);
  }
  
  easyhandle = curl_easy_init();
  curl = curl_easy_init();
  
  if(!curl) {
	fprintf(stderr, "curl_easy_init() failed, in %s at line #%d\n", __FILE__, __LINE__);
	exit(EXIT_FAILURE);
  }
  
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
  res = curl_easy_perform(curl);
  if(res != CURLE_OK){
	fprintf(stderr, "curl_easy_perform() failed : %s, in %s at line #%d\n", curl_easy_strerror(res), __FILE__, __LINE__);
    curl_easy_cleanup(curl);
	fclose(file);
	exit(EXIT_FAILURE);
  }
  
  curl_easy_cleanup(curl);
  fclose(file);
}
