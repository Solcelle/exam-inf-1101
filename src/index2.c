
#include <string.h>

#include "index.h"
#include "map.h"
#include "printing.h"
#include "trie.h"
#include "ctype.h"


struct index
{
	map_t *map;
	list_t *maps;
	list_t *doc;
	list_t *doc_length;
	char *curr_doc;
};


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


static inline int cmp_strs(void *a, void *b)
{
	return strcasecmp((const char *)a, (const char *)b);
}


index_t *index_create()
{
	index_t *idx = malloc(sizeof(index_t));
	idx->maps = list_create(compare_pointers);
	idx->map = map_create(compare_strings, hash_string);
	idx->doc = list_create(compare_pointers);
	idx->doc_length = list_create(compare_pointers);
	idx->curr_doc = NULL;
	return idx;
}


void index_destroy(index_t *index)
{
	
}


void index_add_document(index_t *idx, char *document_name, list_t *words)
{	
	int i;
	char *curr;
	char **content = malloc((sizeof(char *)) * list_size(words));
	list_iter_t *iter = list_createiter(words);
	
	map_t *map = map_create(compare_strings, hash_string);
	
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
		// if (i == 2) {
		// 	DEBUG_PRINT(curr);
		// 	DEBUG_PRINT(key);
		// }
		// char *tmp = "Tragedy";
		// if (!strcmp(curr, tmp)){
		// 	if (!strcmp(curr, key))
		// 		DEBUG_PRINT("%s %d", key, i);
		// 	else
		// 		DEBUG_PRINT("%s", key);
		// }
		// if (!strcmp(curr, key))
		// 	DEBUG_PRINT("%d\n", i);

		// If word is new creates new list in hashmap
		if (!map_haskey(map, key)) {
			// char *tmp = "tragedy";
			// if (!strcmp(key, tmp))
			// 	DEBUG_PRINT("%d", i);
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
	list_addlast(idx->maps, map);

	// Adds content of document to idx
	list_addlast(idx->doc, content);
	void *p_doc_len = malloc(sizeof(int));
	*((int*)p_doc_len) = i;
	list_addlast(idx->doc_length, p_doc_len);

	// char *s = "the";
	// void *test_list = map_get(idx->map, s);
	// printf("number of 'the': %d\n", list_size(test_list));

	free(document_name);
}


search_result_t *index_find(index_t *idx, char *query)
{
	search_result_t *res = malloc(sizeof(search_result_t));
	search_hit_t *hit = malloc(sizeof(search_hit_t));
	res->hit = hit;

	DEBUG_PRINT("%d", list_size(idx->maps));

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


char *autocomplete(index_t *idx, char *input, size_t size)
{
	return NULL;
}


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
