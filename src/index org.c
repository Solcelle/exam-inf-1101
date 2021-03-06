
#include <string.h>

#include "index.h"
#include "map.h"
#include "printing.h"
#include "trie.h"
#include "ctype.h"

/*
 * Implement your index here.
 */ 
struct index
{
	map_t *map;
	list_t *doc;
	list_t *doc_length;
};

/*
 * Struct to hold a single search result.
 * Implement this to work with the search result functions.
 */
struct search_result
{
	list_t *index;
	list_iter_t *index_iter;
	list_t *doc;
	list_t *doc_length;
	search_hit_t *hit;
};


static inline int cmp_ints(void *a, void *b)
{
	return *((int *)a) - *((int *)b);
}

/*
 * Compares two strings without case-sensitivity
 */ 
static inline int cmp_strs(void *a, void *b)
{
	return strcasecmp((const char *)a, (const char *)b);
}



/*
 * Creates a new, empty index.
 */
index_t *index_create()
{
	index_t *idx = malloc(sizeof(index_t));
	idx->map = map_create(compare_strings, hash_string);
	idx->doc = list_create(compare_pointers);
	idx->doc_length = list_create(compare_pointers);
	return idx;
}


/*
 * Destroys the given index.  Subsequently accessing the index will
 * lead to undefined behavior.
 */
void index_destroy(index_t *index)
{
	
}


/*
 * Adds all the words from the given document to the given index.
 * This function is responsible for deallocating the list and the document name after use.
 */
void index_add_document(index_t *idx, list_t *words)
{	
	DEBUG_PRINT("here");
	int i;
	char *curr;
	char **content = malloc((sizeof(char *)) * list_size(words));
	list_iter_t *iter = list_createiter(words);

	// Loops through all words
	for (i = 0; list_hasnext(iter); i++) {
		curr = (char *) list_next(iter);

		// Adds word to content
		content[i] = curr;
		
		int len = (int) strlen(curr);
		char *key = malloc(sizeof(char) * len);

		// Removes uppercase letters from key
		for (int i = 0; i < len; i++) {
            if (isupper(curr[i]))  
                key[i] = tolower(curr[i]);
			else
			key[i] = curr[i];
        }

		// If word is new creates new list in hashmap
		if (!map_haskey(idx->map, key)) {
			// char *tmp = "tragedy";
			// if (!strcmp(key, tmp))
			// 	DEBUG_PRINT("%d", i);
			list_t *new_list = list_create(compare_pointers);
			void *p_index = malloc(sizeof(int));
			*((int*)p_index) = i;
			list_addlast(new_list, p_index);
			map_put(idx->map, key, new_list);
		}
		// Else add word to corresponding list in hashmap
		else {
			void *list = map_get(idx->map, key);
			void *p_index = malloc(sizeof(int));
			*((int*)p_index) = i;
			list_addlast(list, p_index);
		}
	}
	// Adds content of document to idx
	list_addlast(idx->doc, content);
	void *p_doc_len = malloc(sizeof(int));
	*((int*)p_doc_len) = i;
	list_addlast(idx->doc_length, p_doc_len);
}


/*
 * Finds a query in the documents in the index.
 * The result is returned as a search_result_t which is later used to iterate the results.
 */
search_result_t *index_find(index_t *idx, char *query)
{
	search_result_t *res = malloc(sizeof(search_result_t));
	search_hit_t *hit = malloc(sizeof(search_hit_t));
	res->hit = hit;

	DEBUG_PRINT(query);

	// Gets location list of query from index to result
	if (map_haskey(idx->map, query)) {
		void *list = map_get(idx->map, query);
		// Update result
		res->index = list;
		res->index_iter = list_createiter(res->index);
		res->hit->len = (int) strlen(query);		
		res->hit->location = 0;
		res->doc = idx->doc;
		res->doc_length = idx->doc_length;
		
		return res;
	}
	
	else {
		DEBUG_PRINT("Query not found in document\n");
		return NULL;
	}
	
}


/*
 * Autocomplete searches the given trie for a word containing input.
 * The input string is NULL terminated and contains size letters (excluding null termination).
 * The output string MUST be null terminated.
 */ 
char *autocomplete(index_t *idx, char *input, size_t size)
{
	return NULL;
}


/* 
 * Return the content of the current document.
 * Subsequent calls to this function should return the next document.
 * This function should only be called once for each document.
 * This function should return NULL if there are no more documents.
 */
char **result_get_content(search_result_t *res)
{
	list_iter_t *iter = list_createiter(res->doc);
	if (list_hasnext(iter)) {
		char **content = (char **) list_popfirst(res->doc);
		return content;
	}
	else {
		return NULL;
	}
}


/*
 * Get the length of the current document.
 * Subsequent calls should return the length of the same document.
 */
int result_get_content_length(search_result_t *res)
{
	list_iter_t *iter = list_createiter(res->doc_length);
	if (list_hasnext(iter)) {
		void *i = list_popfirst(res->doc_length);
		int length = *(int*) i;
		return length;
	}
	else {
		return NULL;
	}
}


/*
 * Get the next result from the current query.
 * The result should be returned as an int, which is the index in the document content.
 * Should return NULL at the end of the search results.
 */
search_hit_t *result_next(search_result_t *res)
{
	if (list_hasnext(res->index_iter)) {
		void *i = list_next(res->index_iter);
		res->hit->location = *(int*) i;
		DEBUG_PRINT("location: %i\n", *(int*) i);
		return res->hit;
	}

	else {
		return NULL;
	}
}
