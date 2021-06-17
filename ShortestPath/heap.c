#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>


typedef struct HEAP heap;
struct heap
{
    int val;
    double cost;
    struct heap *left;
    struct heap *right;
    int nb;
};

struct heap *initheap()
{
    struct heap *h = malloc(sizeof(*h));
    if (h)
    {
    	h->val = -1;
        h->left = NULL;
        h->right = NULL;
        h->nb = 0;
    }
    return h;
}

int heap_isempty(struct heap* h)
{
	if (h->right == NULL)
	{
		return 1;
	}
	return 0;
}

void heap_update(struct heap* h, int val, double cost)
{
	struct heap *n = initheap();
	n->val = val;
	n->cost = cost;

	int r = 1;

    if (h->right == NULL)
	{
		h->right = n;
	}
	else if (cost < h->right->cost)
	{
		n->nb = h->right->nb + 1;
		n->right = h->right;
		h->right = n;
	}
	else
	{
		struct heap* t = h->right;
		while (t->right != NULL && t->left != NULL)
		{
			if (t->right->nb <= t->left->nb)
			{
				if (cost < t->right->cost)
				{
					n->right = t->right;
					t->right = n;
					t->nb += 1;
					if (n->right != NULL)
					{
						n->nb = n->right->nb + 1;
					}
					return;
				}
				else
				{
					t->nb += 1;
					t = t->right;
					if (r)
					{
						h = h->right;
					}
					else
					{
						h = h->left;
					}
					r = 1;
				}
			}
			else
			{
				if (cost < t->left->cost)
				{
					n->left = t->left;
					t->left = n;
					t->nb += 1;
					if (n->left != NULL)
					{
						n->nb = n->left->nb + 1;
					}
					return;
				}
				else
				{
					t->nb += 1;
					t = t->left;
					if (r)
					{
						h = h->right;
					}
					else
					{
						h = h->left;
					}
					r = 0;
				}
			}
		}
		if (t->right == NULL)
		{
			if (cost < t->cost)
			{
				n->nb = t->nb + 1;
				n->right = t;
				if (r)
				{
					h->right = n;
				}
				else
				{
					h->left = n;
				}
			}
			else
			{
				t->nb += 1;
				t->right = n;
			}
		}
		else if (t->left == NULL)
		{
			if (cost < t->cost)
			{
				n->nb = t->nb + 1;
				n->left = t;

				if (r)
				{
					h->right = n;
				}
				else
				{
					h->left = n;
				}
			}
			else
			{
				t->nb += 1;
				t->left = n;
			}
		}
	}
}


void stab(struct heap* h, int r)
{
	if (r)
	{
		if (h->right == NULL)
		{
			h->right = h->left;
		}
		else if (h->left == NULL)
		{
		
		}
		else
		{
			if (h->right->cost < h->left->cost)
			{
				stab(h->right, 1);
				h->right->left = h->left;
			}
			else
			{
				stab(h->left, 0);
				h->left->right = h->right;
				h->right = h->left;
			}
		}
	}
	else
	{
		if (h->left == NULL)
		{
			h->left = h->right;
		}
		else if (h->right == NULL)
		{
		
		}
		else
		{
			if (h->right->cost < h->left->cost)
			{
				stab(h->right, 1);
				h->right->left = h->left;
			}
			else
			{
				stab(h->left, 0);
				h->left->right = h->right;
				h->right = h->left;
			}
		}
	}
}

/*
void stabr(struct heap* h)
{
	if (h->right == NULL)
	{
		h->right = h->left;
	}
	else if (h->left == NULL)
	{
		
	}
	else
	{
		if (h->right->cost < h->left->cost)
		{
			stabr(h->right);
			h->right->left = h->left;
		}
		else
		{
			stabl(h->left);
			h->left->right = h->right;
		}
	}
}

void stabl(struct heap* h)
{
	if (h->left == NULL)
	{
		h->left = h->right;
	}
	else if (h->right == NULL)
	{
		
	}
	else
	{
		if (h->right->cost < h->left->cost)
		{
			stabr(h->right);
			h->right->left = h->left;
		}
		else
		{
			stabl(h->left);
			h->left->right = h->right;
		}
	}
}
*/

void heap_pop(struct heap* h, int* val, double* cost)
{
	if (h->right == NULL)
	{
		printf("heap vide\n");
		return;
	}

	//h = h->right;

	*val = h->right->val;
	*cost = h->right->cost;


	if (h->right->left == NULL)
	{
		if (h->right->right == NULL)
		{
			h->right = NULL;
		}
		else
		{
			h->right = h->right->right;
		}
	}
	else
	{
		if (h->right->right == NULL)
		{
			h->right = h->right->left;
		}
		else
		{
			if (h->right->right->cost < h->right->left->cost)
			{
				stab(h->right->right, 1);
				
				h->right->right->left = h->right->left;
				h->right = h->right->right;
			}
			else
			{
				stab(h->right->left, 0);
				
				h->right->left->right = h->right->right;
				h->right = h->right->left;
			}
		}
	}
}


void _printheap(struct heap* h, int t)
{
	for (int i = 0; i < t; ++i)
	{
		printf("-");
	}

	printf("%i\n", h->val);
	if (h->left != NULL)
	{
		//printf("l");
		_printheap(h->left, t + 1);
	}
	if (h->right != NULL)
	{
		//printf("r");
		_printheap(h->right, t + 1);
	}
}

void heap_print(struct heap* h)
{
	if (h->right == NULL)
	{
		printf("heap vide\n");
		return;
	}
	h = h->right;
	/*for (int i = 0; i < t; ++i)
	{
		printf("-");
	}
	*/
	printf("%i\n", h->val);
	if (h->left != NULL)
	{
		//printf("l");
		_printheap(h->left, 1);
	}
	if (h->right != NULL)
	{
		//printf("r");
		_printheap(h->right, 1);
	}
}