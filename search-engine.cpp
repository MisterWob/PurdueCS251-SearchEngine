
#include <string.h>
#include "webcrawl.h"
#include "search-engine.h"
#include <cstdlib>

DictionaryType dt;
int maxURLs = 1000;

int port_href;

SearchEngine::SearchEngine( int port, DictionaryType dictionaryType):
  MiniHTTPD(port)
{
  // Create dictionary of the indicated type
	if(dictionaryType == HashDictionaryType) _wordToURLList = new HashDictionary();
	else if(dictionaryType == ArrayDictionaryType) _wordToURLList = new ArrayDictionary();
	else if(dictionaryType == AVLDictionaryType) _wordToURLList = new AVLDictionary();
	else if(dictionaryType == BinarySearchDictionaryType) _wordToURLList = new BinarySearchDictionary();
	else _wordToURLList = NULL;
	
	dt = dictionaryType;
  // Populate dictionary and sort it if necessary
  
  FILE *file = fopen("url.txt", "r");
  
  char * local_buffer = (char*) malloc(5000);
  strcpy(local_buffer, "");
  int maxURLs = 1001;
  URLRecord ** records = new URLRecord*[maxURLs];
  
  for(int i = 0; i < maxURLs; i++) {
  	records[i] = new URLRecord();
  }
  
  
  
  int c;
  int i = 0;
  int ncounter = 0;
  
  

  
  while(fgets(local_buffer,500, file)) {
  	if(strcmp(local_buffer, "\n") != 0) {
  		
  		//Buffer for one url and description
  		char * t = (char*)malloc(700);
  		t = strtok(local_buffer, "\n");
  		
  		char * lb = strdup(local_buffer);
		
		//string for index
		
		char * i_string = (char*) malloc(100);
		strcpy(i_string,"");
		char * is = i_string;
		int index = 0;
		
		//Getting Index
		
		while(*lb != ' ') {
			*is = *lb;
			is++; lb++;
		}
		*is = '\0';
		lb++; //removes whitespace
		index = atoi(i_string);
		
		//Getting URL
		
		char * url = strtok(lb, "\n");
		url = strdup(url);
		
		//Getting Description
		fgets(local_buffer, 500, file);
		
		char * desc = (char*) malloc(500);
		t = strtok(local_buffer, "\n");
		desc = strdup(t);
	
		records[index]->_url = strdup(url);
		records[index]->_description = strdup(desc);
		
		//printf("%d url:%s\ndesc:%s\n\n", index, records[i]->_url, records[i]->_description);
		
  	}
  }

printf("\n\nurl.txt populated...\n");
	free(local_buffer);
	//free(url);
	//free(desc);
	fclose(file);
	

	
	file = fopen("word.txt", "r");
	local_buffer = (char*) malloc(500);
	
	while(fgets(local_buffer, 500, file)) {
		if(strcmp(local_buffer, "\n") != '\0') {
			
			//Getting word
			char * t = (char*)malloc(500);
			t = strtok(local_buffer, " ");
			char * word = (char*) malloc(100);
			word = strdup(t);
			//printf("word: %s ", word);
			//Getting indices
			char * numString = strtok(NULL, "\n");
			char * ns = strdup(numString);
			int j = 0;
			int ch;
			
			URLRecordList * _head;
			_head = NULL;
			URLRecordList * _prev;
			_prev = NULL;
			
			char * index_string = (char*) malloc(20);
			
			//Gets individual indices
			while((ch = *ns) != '\0') {
				if(ch != ' ') {
					index_string[j] = ch; j++;
				}
				else {
					if(j > 0) {
						
						index_string[j] = '\0';
						
						j = 0;
						
						int p = atoi(index_string);
						//printf("%d ", index);
						if(records[p]->_url == NULL) continue;
						
							URLRecordList * newNode = new URLRecordList();
							
							if(_head == NULL) _head = newNode;
							
							newNode->_urlRecord = records[p];
							newNode->_next = NULL;
							
							if(_prev != NULL) _prev->_next = newNode;
							
							
							_prev = newNode;
							
						
						
						
					}
				}
				ns++;
			}
			
			_wordToURLList->addRecord(word, (URLRecordList*)_head);
			//_________________________________________________________
						
		}
		//printf("\n");
	}
	fclose(file);
	printf("word.txt populated...\n");
}

void
SearchEngine::dispatch( FILE * fout, const char * documentRequested)
{

   if (strcmp(documentRequested, "/")==0) {
    // Send initial form
    fprintf(fout, "<TITLE>Search v1.0</TITLE>\r\n");
    fprintf(fout, "<BODY background=\"https://s-media-cache-ak0.pinimg.com/originals/f2/e9/53/f2e95302ce9ce1c1a0356fe5bed478b1.jpg\">\n");
    fprintf(fout, "<CENTER><font color=\"white\" font size=\"200\" font face=\"Helvetica\">Quadro Search</font><CENTER>\n");
    fprintf(fout, "<H2>\n");
    fprintf(fout, "<font color=\"gray\" font size=\"5\" font face=\"Helvetica\">v1.0</font>");
    fprintf(fout, "<FORM ACTION=\"search\">\n");
    fprintf(fout, "<INPUT TYPE=\"text\" NAME=\"word\" MAXLENGTH=\"1000\"><P>\n");
    fprintf(fout, "<p><font color =\"white\"font size=\"5\">&reg;Yash Shiroya | 2015</font></p>");
    fprintf(fout, "</H2>\n");
    fprintf(fout, "</FORM></CENTER></BODY>\n");
    return;
  }

  // TODO: The words to search in "documentRequested" are in the form
  // /search?word=a+b+c
  //
  // You need to separate the words before search
  // Search the words in the dictionary and find the URLs that
  // are common for al the words. Then print the URLs and descriptions
  // in HTML. Make the output look nicer.

  // Here the URLs printed are hardwired
	
	//Checking sanity of search request
  
  if(strstr(documentRequested, "/search?word=") == NULL) {
  	return;
  }
  else {
  	printf("%s\n", documentRequested);
  }
  
  char * search = strdup(documentRequested);
  
  //Extracting separate words______________________________________
  
  char * word = (char*) malloc(100);
  word = search + 13;
  char * w = word;
  
  //printf("search: %s\n",word);
  
  char ** word_list = new char*[500]; 
  
  for(int i = 0; i < 500; i++) {
  	word_list[i] = NULL;
  }
  
  int wordCount;
  wordCount = 0;

  printf("\n");
  int ch;
  
  char word_ext[50]; int l = 0;
  
  char * cat_string = (char*) malloc(1000);
  strcpy(cat_string,"");
  //nextword___________________________________________
  
  while((ch = *w) != '\0') {
  	if(ch != '+') {
	        word_ext[l++] = ch;
	}
	else {
		//w++;
		if(l > 0) {
	        word_ext[l] = '\0';
	        l = 0;
			word_list[wordCount] = strdup(word_ext);
			//printf("word: %s\n", word_list[wordCount]);
			strcat(cat_string, word_ext);
			strcat(cat_string, " ");
			wordCount++;
		}
	}
  	w++;
  }
  
  //Getting last word out
  if(ch == '\0') {
  		word_ext[l] = '\0';
	    l = 0;
		word_list[wordCount] = strdup(word_ext);
		//printf("word: %s\n", word_list[wordCount]);
		strcat(cat_string, word_ext);
		wordCount++;
  }
  
 
  //____________________________________________________
  
  //_________________________________________________________________
//printf("cat_string %s\n", cat_string);
    
  //const int nurls=2;

  //const char * words = "data structures";

  //fprintf( stderr, "Search for words: \"%s\"\n", words);

  fprintf( fout, "<TITLE>Search Results</TITLE>\r\n");
  fprintf(fout, "<BODY background=\"http://7-themes.com/data_images/out/73/7021624-modern-architecture-wallpaper-for-desktop.jpg\">\n");
  fprintf( fout, "<H1> <Center>Boiler Search</H1>\n");
  fprintf( fout, "<H2> Search Results for \"%s\"</center></H2>\n", cat_string );
  //fprintf(fout, "</BODY>");

  URLRecord ** url_list = new URLRecord * [500];
  int count1 = 0;
  int count2 = 0;
  
 
  for(int m = 0; m < wordCount; m++) {
  	
  	URLRecordList * _record = (URLRecordList*)_wordToURLList->findRecord(word_list[m]);  
  	 
  	 
  	 while(_record != NULL) {
  	 	
  	 	
  	 	//int seen = 0;
  	 	bool seen = false;
  	 	for(int j = 0; j < count1; j++) {
  	 		
  	 		if(url_list[j] == _record->_urlRecord) {// Make changes to add better check
  	 			seen = true;
  	 			break;
  	 		}
  	 		
  	 	}
  	 	
  	 	if(seen == false) {
  	 		url_list[count1] = _record->_urlRecord;
  	 		count1++;
  	 	}
  	 	_record = _record->_next;	
  	 	
  	 }
  }
    for ( int i = 0; i < count1; i++ ) {
    	//if(url_list[i] == NULL) continue;
    	printf("url: %s\n", url_list[i]->_url);
    }
  
  for(int t = 0; t < count1; t++) {
  	for(int k = 0; k < wordCount; k++) {
  		URLRecordList * r = (URLRecordList*)_wordToURLList->findRecord(word_list[k]);
  		bool seen = false;
  		
  		while(r != NULL) {
  			if(r->_urlRecord == url_list[t]) {
  				seen = true;
  			}
  			r = r->_next;
  		}
  		
  		if(seen == false) {
  			url_list[t] = NULL;
  		}
  	}
  }
  
  //int _index = 0;

  for ( int i = 0; i < count1; i++ ) {
  
  if(url_list[i] == NULL) continue;
  
    	fprintf( fout, "<h3>%d. <a href=\"%s\">%s</a><h3>\n", count2+1, url_list[i]->_url, url_list[i]->_url );
    	fprintf( fout, "<font color=\"gray\" font face=\"Helvetica\">%s</font><p>\n", url_list[i]->_description);
    	count2++;
    
  }

  
	fprintf(fout, "<h3><marquee><a href=\"http://data.cs.purdue.edu:%d/\">Search Home</a></marquee><h3>\n", port_href);
  // Add search form at the end
  fprintf(fout, "<HR><H2>\n");
  fprintf(fout, "<FORM ACTION=\"search\">\n");
  fprintf(fout, "Search:\n");
  fprintf(fout, "<INPUT TYPE=\"text\" NAME=\"word\" MAXLENGTH=\"80\"><P>\n");
  fprintf(fout, "</H2>\n");
  fprintf(fout, "</FORM>\n");
  fprintf(fout, "</BODY>");
}

void
printUsage()
{
  const char * usage =
    "Usage: search-engine port (array | hash | avl | bsearch)\n"
    "  It starts a search engine at this port using the\n"
    "  data structure indicated. Port has to be larger than 1024.\n";

  fprintf(stderr, "%s", usage);
}


int main(int argc, char ** argv)
{
  if (argc < 3) {
    printUsage();
    return 1;
  }

  // Get port
  int port;
  sscanf( argv[1], "%d", &port);
  port_href = port;
  // Get DictionaryType
  const char * dictType = argv[2];
  DictionaryType dictionaryType;
  if (!strcmp(dictType, "array")) {
    dictionaryType = ArrayDictionaryType;
  }
  else if (!strcmp(dictType, "hash")) {
    dictionaryType = HashDictionaryType;
  }
  else if (!strcmp(dictType, "avl")) {
    dictionaryType = AVLDictionaryType;
  }
  else if (!strcmp(dictType, "bsearch")) {
    dictionaryType = BinarySearchDictionaryType;
  }
  else {
    printUsage();
    return 0;
  }
  
  SearchEngine httpd(port, dictionaryType);
  
  httpd.run();

  return 0;
}
