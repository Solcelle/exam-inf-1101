
#include <string.h>

#include "index.h"
#include "map.h"
#include "printing.h"
#include "trie.h"

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
	char **content;
	list_t *hit_list;
	list_t *doc;
	list_t *doc_length;
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
void index_add_document(index_t *idx, char *document_name, list_t *words)
{	
	int i;
	void *curr;
	char **content = malloc((sizeof(char *)) * list_size(words));
	// idx->content = malloc((sizeof(char *)) * list_size(words));
	list_iter_t *iter = list_createiter(words);

	// Loops through all words
	for (i = 0; list_hasnext(words); i++) {
		curr = (char *) list_next(words);

		// Adds word to content
		content[i] = curr;

		// If word is new creates new list in hashmap
		if (!map_haskey(idx->map, curr)) {
			list_t *new_list = list_create(compare_pointers);
			list_addlast(new_list, i);
			map_put(idx->map, curr, new_list);
		}
		// Else add word to corresponding list in hashmap
		else {
			void *list = map_get(idx->map, curr);
			list_addlast(list, i);
		}
	}
	// Adds content of document to idx
	list_addlast(idx->doc, content);
	list_addlast(idx->doc_length, i);

	// char *s = "the";
	// void *test_list = map_get(idx->map, s);
	// printf("number of 'the': %d\n", list_size(test_list));

	free(document_name);
}


/*
 * Finds a query in the documents in the index.
 * The result is returned as a search_result_t which is later used to iterate the results.
 */
search_result_t *index_find(index_t *idx, char *query)
{
	search_result_t *res = malloc(sizeof(search_result_t));

	if (map_haskey(idx->map, query)) {
		void *list = map_get(idx->map, query);
		res->hit_list = list;
	}
	res->doc = idx->doc;
	res->doc_length = idx->doc_length;

	return res;
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
		char **content = (char **) list_popfirst(iter);
		DEBUG_PRINT(content[6]);
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
	list_iter_t *iter = list_createiter(res->doc);
	if (list_hasnext(iter)) {
		int length = list_popfirst(iter);
		DEBUG_PRINT("%d\n", length);
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


	return NULL;
}
