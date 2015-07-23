
#include <string.h>
#include "webcrawl.h"
#include "search-engine.h"
#include <cstdlib>

DictionaryType dt;
int maxURLs = 1000;

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
  // display initial form
	if (strcmp(documentRequested, "/")==0)
	{
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
	
	// ignore if this is not a search
	if (strlen(documentRequested) < 13)
		return;
	
	// extract query string
	
	char *param = new char[strlen(documentRequested)];
	strcpy(param, documentRequested+13);

	// initialize words list
	char **words = new char*[100];
	int i;
	for (i = 0; i < 100; i++)
	{
		words[i] = NULL;
	}
	
	// parse url for search parameters
	int numWords;
	numWords = 0;
	
	char *word;
	word = strtok(param, "+");
	
	while (word != NULL)
	{
		words[numWords] = new char[50];
		
		strcpy(words[numWords], word);
		numWords++;
		
		word = strtok(NULL, "+");
	}

	// create friendly search string
	char *query = new char[100*50];
	strcpy(query, "");
	
	for (i = 0; i < numWords; i++)
	{
		strcat(query, words[i]);
		
		if (i < numWords - 1)
			strcat(query, " ");
	}
	
	// print search results
	fprintf(stderr, "Search for words: \"%s\"\n", query);

	fprintf(fout, "<TITLE>Search Results</TITLE>\r\n");
	fprintf(fout, "<H1> <Center><em>Boiler Search</em></H1>\n");
	fprintf(fout, "<H2> Search Results for \"%s\"</center></H2>\n", query);
	
	
	
	int counter;
	counter = 0;
	
	int counter1;
	counter1 = 0;
	
	URLRecord **list = new URLRecord*[500];
	
	
	
	for (i = 0; i < numWords; i++)
	{
		URLRecordList * data = (URLRecordList*)_wordToURLList->findRecord(words[i]);  
  	 
  	 
  	 while(data != NULL) {
  	 	
  	 	
  	 	int exists = 0;
  	 	
  	 	for(int j = 0; j < counter1; j++) {
  	 		
  	 		if(list[j] == data->_urlRecord) {// Make changes to add better check
  	 			exists = 1;
  	 			break;
  	 		}
  	 		
  	 	}
  	 	
  	 	if(exists == 0) {
  	 		list[counter1] = data->_urlRecord;
  	 		counter1++;
  	 	}
  	 	data = data->_next;	
  	 	
  	 }
	}
	
	/*for (i = 0; i < listCount; i++)
	{
		int j;
		for (j = 0; j < numWords; j++)
		{
			URLRecordList* data;
			data = (URLRecordList*)_wordToURLList->findRecord(words[j]);
			
			int exists = 0;
			
			while (data != NULL)
			{
				if (data->_urlRecord == list[i])
				{
					exists = 1;
				}
				
				data = data->_next;
			}
			
			if (exists == 0)
				list[i] = NULL;
		}
	}*/
		
	
	for (i = 0; i < counter1; i++)
	{
		if (list[i] == NULL) continue;
		
		fprintf(fout, "<h3>%d. <a href=\"%s\">%s</a><h3>\n", counter+1, list[i]->_url, list[i]->_url);
		fprintf(fout, "<blockquote>%s<p></blockquote>\n", list[i]->_description);
	
		counter++;
	}
	
	
	if (dt == ArrayDictionaryType)
		fprintf(fout, "<h3>Dictionary Used: Array Dictionary</h3>\n");
	else if (dt == HashDictionaryType)
		fprintf(fout, "<h3>Dictionary Used: Hash Dictionary</h3>\n");
	else if (dt == AVLDictionaryType)
		fprintf(fout, "<h3>Dictionary Used: AVL Dictionary</h3>\n");
	else if (dt == BinarySearchDictionaryType)
		fprintf(fout, "<h3>Dictionary Used: Binary Search Dictionary</h3>\n");
	else
		fprintf(fout, "<h3>Dictionary Used: Unknown Dictionary</h3>\n");
	
	// add search form at the end
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
