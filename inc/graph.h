/******************************************************************
* This class is used to represent the LDPC's Tanner Graph. Since  *
* LDPC graphs are by definition sparse, the graph is represented  *
* through adjacency lists.                                        *
******************************************************************/
#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <list>
#include <iostream>

enum node_type { VARIABLE, CHECK };

/* forward declaration */
class edge;
class node;

class edge{
	private:
	node &dest;

	public:
	/* constructors */
	edge(node& dest);

	/* copy constructor */
	edge(const edge& rhs);

	/* destructor */
	~edge(void);

	/* assignment op */
	edge& operator = (const edge rhs);
		
	
	/* methods */
	node& get_dest(void) const;
	void set_dest(node& dest);
};

class node{

	private:
	node_type t;
	int i;
	std::list<edge> edges;
	
	public:
	/* constructors */
	node();
	node(node_type t, int i);
	node(const node& rhs);
	
	/* destructor */
	~node(void);
	
	/* assignment op */
	node& operator = (const node rhs);

	/* print op */
	friend std::ostream& operator << (std::ostream& os, const node& v);
	
	/* info methods */
	void set_type(node_type t);
	void set_idx(int i);
	node_type get_type(void) const;
	int get_idx(void) const;

	/* edge methods */
	void add_edge(node& dest);
	void add_bidirectional_edge(node& dest);
	std::list<edge> get_edges(void);
};

#endif
