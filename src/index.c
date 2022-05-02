
#include <string.h>

#include "index.h"
#include "map.h"
#include "printing.h"
#include "trie.h"
#include "ctype.h"


struct index
{
	list_t *docs;
};

struct document
{
	map_t *map;
	char **words;
	int length;
	list_t *curr_idx;
	list_iter_t *curr_iter;
};

struct search_result
{
	list_t *docs;
	list_iter_t *doc_iter;
	document_t *curr_doc;
	search_hit_t *hit;
};


static inline int cmp_ints(void *a, void *b)
{
	return *((int *)a) - *((int *)b);
}


static inline int cmp_strs(void *a, void *b)
{
	return strcasecmp((const char *)a, (const char *)b);
}


index_t *index_create()
{
	index_t *idx = malloc(sizeof(index_t));
	idx->docs = list_create(compare_pointers);
	return idx;
}


void index_destroy(index_t *index)
{
	
}


void index_add_document(index_t *idx, char *document_name, list_t *words)
{	
	int i, len = 0;
	char *curr;
	char **content = malloc((sizeof(char *)) * list_size(words));
	map_t *map = map_create(compare_strings, hash_string);
	list_iter_t *iter = list_createiter(words);

	
	// Loops through all words
	for (i = 0; list_hasnext(iter); i++) {
		curr = (char *) list_next(iter);

		// Adds word to content
		content[i] = curr;
		
		len = (int) strlen(curr);
		char *key = malloc(sizeof(char) * len);

		// Removes uppercase letters from key
		for (int i = 0; i < len; i++) {
            if (isupper(curr[i]))  
                key[i] = tolower(curr[i]);
			else
			key[i] = curr[i];
        }

		// If word is new creates new list in hashmap
		if (!map_haskey(map, key)) {
			list_t *new_list = list_create(compare_pointers);
			void *p_index = malloc(sizeof(int));
			*((int*)p_index) = i;
			list_addlast(new_list, p_index);
			map_put(map, key, new_list);
		}
		// Else add word to corresponding list in hashmap
		else {
			void *list = map_get(map, key);
			void *p_index = malloc(sizeof(int));
			*((int*)p_index) = i;
			list_addlast(list, p_index);
		}
	}
	// Adds documents info to doc struct
	document_t *doc = malloc(sizeof(document_t));
	doc->map = map;
	doc->words = content;
	doc->length = i;

	// Adds doc struct to index
	list_addlast(idx->docs, doc);

	free(document_name);
}


search_result_t *index_find(index_t *idx, char *query)
{
	search_result_t *res = malloc(sizeof(search_result_t));
	res->docs = list_create(compare_pointers);
	search_hit_t *hit = malloc(sizeof(search_hit_t));
	res->hit = hit;

	list_iter_t *iter = list_createiter(idx->docs);
	document_t *doc;

	while (list_hasnext(iter)) 
	{
		doc = (document_t*) list_next(iter);
		if (map_haskey(doc->map, query)) {
			DEBUG_PRINT("Querey found in doc\n");
			void *list = map_get(doc->map, query);
			doc->curr_idx = list;
			doc->curr_iter = list_createiter(doc->curr_idx);
			list_addlast(res->docs, doc);
		}
		
		else {
			DEBUG_PRINT("Query not found in document\n");
		}
	}
	res->doc_iter = list_createiter(res->docs);
	res->hit->len = (int) strlen(query);		
	res->hit->location = 0;
	return res;
}


char *autocomplete(index_t *idx, char *input, size_t size)
{
	return NULL;
}


char **result_get_content(search_result_t *res)
{
	if (list_hasnext(res->doc_iter)) {
		res->curr_doc = list_next(res->doc_iter);
		return res->curr_doc->words;
	}
	else {
		return NULL;
	}
}


int result_get_content_length(search_result_t *res)
{
	return res->curr_doc->length;
}


search_hit_t *result_next(search_result_t *res)
{

	if (list_hasnext(res->curr_doc->curr_iter)) {
		void *i = list_next(res->curr_doc->curr_iter);
		res->hit->location = *(int*) i;
		DEBUG_PRINT("location: %i\n", *(int*) i);
		return res->hit;
	}

	else {
		return NULL;
	}
}
