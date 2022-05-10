
#include <string.h>
#include "index.h"
#include "map.h"
#include "printing.h"
#include "trie.h"
#include "list.h"
#include "ctype.h"


struct index
{
	list_t *docs;
	trie_t *trie;
};

struct document
{
	char **words;
	map_t *map;
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


index_t *index_create()
{
	index_t *idx = malloc(sizeof(index_t));
	idx->docs = list_create(compare_pointers);
	idx->trie = trie_create();
	return idx;
}


void index_destroy(index_t *index)
{
	// Destroy docs
	list_iter_t *iter = list_createiter(index->docs);
	while (list_hasnext(iter))
	{
		document_t *doc = list_next(iter);

		map_destroy(doc->map, free, (void *) list_destroyfull);
		list_destroyiter(doc->curr_iter);

		free(doc->words);

		free(doc);
	}

	// Destroy trie
	trie_destroy(index->trie);

	// Destory list of docs
	list_destroy(index->docs);

	free(index);
}


void index_add_document(index_t *idx, list_t *words)
{	
	int i, len = 0;
	char *curr;
	char **content = malloc((sizeof(char *)) * list_size(words));
	map_t *map = map_create(compare_strings, hash_string);
	list_iter_t *iter = list_createiter(words);
	

	// Loops through all words
	for (i = 0; list_hasnext(iter); i++)
	{
		curr = (char *) list_next(iter);
		content[i] = curr;
		len = (int) strlen(curr);
		char *key = malloc(sizeof(char) * len);

		// Removes uppercase letters from key
		for (int i = 0; i < len; i++)
		{
            if (isupper(curr[i]))  
                key[i] = tolower(curr[i]);
			else
				key[i] = curr[i];
        }

		// If word is new creates new list in hashmap
		if (!map_haskey(map, key))
		{
			list_t *new_list = list_create(compare_pointers);
			int *p_index = malloc(sizeof(int));
			*p_index = i;
			list_addlast(new_list, p_index);
			map_put(map, key, new_list);

			// Adds word to trie
			trie_insert(idx->trie, key, NULL);
		}
		// Else add word to corresponding list in hashmap
		else
		{
			void *list = map_get(map, key);
			int *p_index = malloc(sizeof(int));
			*p_index = i;
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
}


search_result_t *index_find(index_t *idx, char *query)
{
	search_result_t *res = malloc(sizeof(search_result_t));
	res->docs = list_create(compare_pointers);
	search_hit_t *hit = malloc(sizeof(search_hit_t));
	res->hit = hit;
	list_iter_t *doc_iter = list_createiter(idx->docs);
	document_t *doc;

	// Loops through documents
	while (list_hasnext(doc_iter)) 
	{
		// Adds query's index to doc and adds doc to res
		doc = (document_t*) list_next(doc_iter);
		if (map_haskey(doc->map, query)) {
			doc->curr_idx = map_get(doc->map, query);
			doc->curr_iter = list_createiter(doc->curr_idx);
			list_addlast(res->docs, doc);
		}
	}

	// If no index was found 
	if (list_size(res->docs) == 0)
	{
		return NULL;
	}

	// Create rest of res
	res->doc_iter = list_createiter(res->docs);
	res->hit->len = (int) strlen(query);		
	res->hit->location = 0;

	return res;
}


char *autocomplete(index_t *idx, char *input, size_t size)
{
	return trie_find(idx->trie, input, size);
}


char **result_get_content(search_result_t *res)
{
	if (res == NULL)
	{
		return NULL;
	}

	// Updateds current doc and returns current doc's words
	if (list_hasnext(res->doc_iter))
	{
		res->curr_doc = list_next(res->doc_iter);
		return res->curr_doc->words;
	}

	else
	{
		return NULL;
	}
}


int result_get_content_length(search_result_t *res)
{
	if (res == NULL)
	{
		return 0;
	}

	return res->curr_doc->length;
}


search_hit_t *result_next(search_result_t *res)
{
	if (res == NULL)
	{
		return NULL;
	}

	// Accesses the next location of the document
	if (list_hasnext(res->curr_doc->curr_iter))
	{
		res->hit->location = *(int*) list_next(res->curr_doc->curr_iter);
		return res->hit;
	}

	// Returns NULL at end of current document
	else
	{
		return NULL;
	}
}
