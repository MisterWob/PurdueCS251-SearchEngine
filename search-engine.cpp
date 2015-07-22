
#include <string.h>
#include "webcrawl.h"
#include "search-engine.h"
#include <cstdlib>

DictionaryType dt;

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
  		
  		char * t = (char*)malloc(700);
  		t = strtok(local_buffer, " \n");
  		
  		char * lb = strdup(local_buffer);
		
		char * i_string = (char*) malloc(100);
		strcpy(i_string,"");
		char * is = i_string;
		//int index = lb[0] - '0';
		//lb += 2;
		
		
		//while(*lb != ' ') {
			//*is = *lb;
			printf("lb %s\n", local_buffer);
			//is++; lb++;
		//}
		
		*is = '\0';
		
		
		char * url;
		url = strdup(lb);
		
		//printf("%s url:%s", i_string, url);
		/*fgets(local_buffer, 500, file);
		
		char * desc = (char*) malloc(500);
		t = strtok(local_buffer, "\n");
		desc = strdup(t);
	
		records[i]->_url = strdup(url);
		records[i]->_description = strdup(desc);
		printf("%d url: %s desc: %s\n\n", index, records[i]->_url, records[i]->_description);*/
		
  	}
  }

	free(local_buffer);
	fclose(file);
	
	/*file = fopen("word.txt", "r");
	local_buffer = (char*) malloc(500);
	
	while(fgets(local_buffer, 500, file)) {
		if(strcmp(local_buffer, "\n") != '\0') {
			
			char * t = (char*)malloc(500);
			t = strtok(local_buffer, " ");
			
			char * word = (char*) malloc(100);
			word = strdup(t);
			
			char * numString = strtok(NULL, " \n");
			char * ns = numString;
			int j = 0;
			int ch;
			char * index = (char*) malloc(20);
			
			while((ch = *ns) != '\0') {
				if(ch != ' ') {
					index[j] = ch; j++;
				}
				else {
					if(j > 0) {
						index[j] = '\0';
						j = 0;
						
					}
				}
			}
			
		}
	}*/
	
}

void
SearchEngine::dispatch( FILE * fout, const char * documentRequested)
{
  if (strcmp(documentRequested, "/")==0) {
    // Send initial form
    fprintf(fout, "<TITLE>CS251 Search</TITLE>\r\n");
    fprintf(fout, "<CENTER><H1><em>Boiler Search</em></H1>\n");
    fprintf(fout, "<H2>\n");
    fprintf(fout, "<FORM ACTION=\"search\">\n");
    fprintf(fout, "Search:\n");
    fprintf(fout, "<INPUT TYPE=\"text\" NAME=\"word\" MAXLENGTH=\"80\"><P>\n");
    fprintf(fout, "</H2>\n");
    fprintf(fout, "</FORM></CENTER>\n");
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
  
  const int nurls=2;

  const char * words = "data structures";

  const char * urls[] = {
    "http://www.cs.purdue.edu",
    "http://www.cs.purdue.edu/homes/cs251"
  };
  
  const char * description[] = {
    "Computer Science Department. Purdue University.",
    "CS251 Data Structures"
  };

  fprintf( stderr, "Search for words: \"%s\"\n", words);

  fprintf( fout, "<TITLE>Search Results</TITLE>\r\n");
  fprintf( fout, "<H1> <Center><em>Boiler Search</em></H1>\n");
  fprintf( fout, "<H2> Search Results for \"%s\"</center></H2>\n", words );

  for ( int i = 0; i < nurls; i++ ) {
    fprintf( fout, "<h3>%d. <a href=\"%s\">%s</a><h3>\n", i+1, urls[i], urls[i] );
    fprintf( fout, "<blockquote>%s<p></blockquote>\n", description[i] );
  }

  // Add search form at the end
  fprintf(fout, "<HR><H2>\n");
  fprintf(fout, "<FORM ACTION=\"search\">\n");
  fprintf(fout, "Search:\n");
  fprintf(fout, "<INPUT TYPE=\"text\" NAME=\"word\" MAXLENGTH=\"80\"><P>\n");
  fprintf(fout, "</H2>\n");
  fprintf(fout, "</FORM>\n");
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
