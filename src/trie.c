#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "trie.h"
#include "printing.h"

#define TRIE_RADIX 26
#define ASCII_TO_IDX(c) c - 97


typedef struct node node_t;
struct node
{
    char *key;
    void *value;
    node_t *children[TRIE_RADIX];
};

struct trie
{
    node_t *root;
};


static inline int isleaf(node_t *node)
{
    // A NULL node is not considered a leaf node
    if (node == NULL)
    {
        return 0;
    }

    for (int i = 0; i < TRIE_RADIX; i++)
    {
        if (node->children[i] != NULL)
        {
            return 0;
        }
    }

    return 1;
}

static node_t *node_create(char *key, void *value)
{
    node_t *node = (node_t *)calloc(1, sizeof(node_t));
    if (node == NULL)
    {
        goto error;
    }

    node->key = key;
    node->value = value;

    return node;

error:
    return NULL;

}


void node_destroy(node_t *node)
{
    free(node);
}

trie_t *trie_create()
{

    trie_t *t = (trie_t *)calloc(1, sizeof(trie_t));

    if (t == NULL)
    {
        goto error;
    }

    t->root = node_create(NULL, NULL);
    return t;

error:
    return NULL;
}

void _trie_destroy(node_t *node)
{
    if (isleaf(node))
    {
        node_destroy(node);
    }
    else
    {
        int i;
        for (i = 0; i < TRIE_RADIX; i++)
        {
            if (node->children[i] != NULL)
            {
                _trie_destroy(node->children[i]);
                node->children[i] = NULL;
            }
        }
        node_destroy(node);
    }

    return;
}


void trie_destroy(trie_t *trie)
{
    _trie_destroy(trie->root);
    free(trie);
    trie = NULL;
}


int trie_insert(trie_t *trie, char *key, void *value)
{
    node_t *iter = trie->root;

    // Only allow alphabet characters
    for (int i = 0; key[i] != '\0'; i++)
    {
        if (!isalpha(key[i]))
        {
            goto error;
        }
    }

    // Find the child indices
    for (int i = 0; key[i] != '\0'; i++)
    {
        // We only use lowercase letters (case insensitive)
        if (iter->children[ASCII_TO_IDX(tolower(key[i]))] == NULL)
        {
            iter->children[ASCII_TO_IDX(tolower(key[i]))] = node_create(NULL, NULL);
        }
        iter = iter->children[ASCII_TO_IDX(tolower(key[i]))];
    }

    iter->key = key;
    iter->value = value;

    return 0;

 error:
    return -1;
}


char *trie_find(trie_t *trie, char *key, size_t size)
{
	node_t *iter = trie->root;

	// Finds the node containing the prefix
	for (int i = 0; i < (int) size; i++)
	{
		if (iter->children[ASCII_TO_IDX(key[i])] == NULL)
		{
			return NULL;
		}
		iter = iter->children[ASCII_TO_IDX(key[i])];
	}

	// Finds the last node after the prefix
	for (int i = 0; i < 26; i++)
	{
		if (iter->children[i] != NULL)
		{
			iter = iter->children[i];
			i = -1;
		}
		if (iter->key != NULL && iter != NULL)
		{
			return iter->key;
		}
	}

	// Returns value of last node
	return iter->key;
}

