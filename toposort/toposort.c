/*
 * toposort - topologically sort library dependencies for mk
 *
 * Usage: toposort dir1 dir2 ...
 *
 * Reads LIBS= from each directory's mkfile, builds dependency graph,
 * outputs directories in dependency order (dependencies first).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXNODES 256
#define MAXLINE 4096
#define MAXPATH 1024

typedef struct Node {
	char *name;
	int ndeps;
	int deps[MAXNODES];
	int state;  /* 0=unvisited, 1=visiting, 2=visited */
} Node;

static Node nodes[MAXNODES];
static int nnodes;
static char *output[MAXNODES];
static int noutput;

static int
findnode(char *name)
{
	int i;
	for(i = 0; i < nnodes; i++)
		if(strcmp(nodes[i].name, name) == 0)
			return i;
	return -1;
}

static int
addnode(char *name)
{
	int i = findnode(name);
	if(i >= 0)
		return i;
	if(nnodes >= MAXNODES){
		fprintf(stderr, "toposort: too many nodes\n");
		exit(1);
	}
	i = nnodes++;
	nodes[i].name = strdup(name);
	nodes[i].ndeps = 0;
	nodes[i].state = 0;
	return i;
}

static void
adddep(int from, int to)
{
	int i;
	Node *n = &nodes[from];
	for(i = 0; i < n->ndeps; i++)
		if(n->deps[i] == to)
			return;
	if(n->ndeps >= MAXNODES){
		fprintf(stderr, "toposort: too many deps\n");
		exit(1);
	}
	n->deps[n->ndeps++] = to;
}

static char*
trim(char *s)
{
	char *e;
	while(isspace(*s)) s++;
	e = s + strlen(s);
	while(e > s && isspace(e[-1])) e--;
	*e = '\0';
	return s;
}

/*
 * Read LIBS= line from mkfile in directory dir
 * Returns allocated string or NULL
 */
static char*
getlibs(char *dir)
{
	char path[MAXPATH];
	char line[MAXLINE];
	FILE *f;
	char *libs = NULL;

	snprintf(path, sizeof path, "%s/mkfile", dir);
	f = fopen(path, "r");
	if(f == NULL)
		return NULL;

	while(fgets(line, sizeof line, f)){
		if(strncmp(line, "LIBS=", 5) == 0){
			libs = strdup(trim(line + 5));
			break;
		}
	}
	fclose(f);
	return libs;
}

/*
 * Parse and add dependencies from a node
 */
static void
parsedeps(int node)
{
	char *libs, *p, *tok;
	int dep;

	libs = getlibs(nodes[node].name);
	if(libs == NULL)
		return;

	p = libs;
	while((tok = strtok(p, " \t\n")) != NULL){
		p = NULL;
		dep = addnode(tok);
		adddep(node, dep);
	}
	free(libs);
}

/*
 * DFS visit for toposort
 */
static int
visit(int n)
{
	int i;
	Node *node = &nodes[n];

	if(node->state == 2)
		return 0;
	if(node->state == 1){
		fprintf(stderr, "toposort: cycle involving %s\n", node->name);
		return 1;
	}

	node->state = 1;

	/* First, discover any new nodes from this node's deps */
	parsedeps(n);

	for(i = 0; i < node->ndeps; i++){
		if(visit(node->deps[i]))
			return 1;
	}

	node->state = 2;
	output[noutput++] = node->name;
	return 0;
}

int
main(int argc, char **argv)
{
	int i, n;

	if(argc < 2){
		/* No args = nothing to sort */
		return 0;
	}

	/* Add initial nodes from command line */
	for(i = 1; i < argc; i++)
		addnode(argv[i]);

	/* Visit each initial node */
	for(i = 1; i < argc; i++){
		n = findnode(argv[i]);
		if(visit(n))
			return 1;
	}

	/* Output in dependency order (dependencies first) */
	for(i = 0; i < noutput; i++){
		printf("%s", output[i]);
		if(i < noutput - 1)
			printf(" ");
	}
	if(noutput > 0)
		printf("\n");

	return 0;
}
