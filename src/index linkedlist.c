
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
	list_t *documents;
};

/*
 * Struct to hold a single search result.
 * Implement this to work with the search result functions.
 */
struct search_result
{
	int content_length;
	list_t *documents;
	list_t *hit_list;
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
	idx->documents = list_create(compare_pointers);
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
	list_iter_t *iter = list_createiter(words);
	list_addfirst(idx->documents, words);
	free(document_name);
}


/*
 * Finds a query in the documents in the index.
 * The result is returned as a search_result_t which is later used to iterate the results.
 */
search_result_t *index_find(index_t *idx, char *query)
{
	search_result_t *res = malloc(sizeof(search_result_t));
	res->documents = list_create(compare_pointers);

	list_iter_t *doc_iter, *word_iter;
	void *curr_doc;
	char *curr_word;

	// Iterate through documents
	doc_iter = list_createiter(idx->documents);

	while (list_hasnext(doc_iter))
		curr_doc = list_next(doc_iter);

		// Iterate through words in document
		word_iter = list_createiter(curr_doc);
		while (list_hasnext(word_iter))
			// DEBUG_PRINT(list_next(word_iter));
			curr_word = list_next(word_iter);

			DEBUG_PRINT(curr_word);
			// Add current word to content

			// If current word is query add to hit list
			if (!strcmp(curr_word, query)) {
				DEBUG_PRINT("MATCH");
			}

	res->content_length = strlen(query);
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
	char **content;
	// res->content;

	return content;
}


/*
 * Get the length of the current document.
 * Subsequent calls should return the length of the same document.
 */
int result_get_content_length(search_result_t *res)
{
	return NULL;
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
