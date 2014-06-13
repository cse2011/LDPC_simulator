#include "graph.h"

/******************************************************************
* Edge functions.                                                 *
******************************************************************/

edge::edge(node  &dest) : dest(dest) {}

edge::edge(const edge& rhs): dest(rhs.dest){}

edge::~edge(void){}

edge& edge::operator = (const edge rhs) {
	dest = rhs.dest;
	return *this;
}

node& edge::get_dest(void) const{
	return dest;
}

void edge::set_dest(node& dest){
	this->dest = dest;
}


/******************************************************************
* Node constructors/destructor                                    *
******************************************************************/

node::node(){
	t = VARIABLE;
	i = 0;
}

node::node(node_type t, int i){
	this->t = t;
	this->i = i;
}

node::node(const node& rhs){
	t = rhs.t;
	i = rhs.i;
	edges = rhs.edges;
}

node::~node(){}

/******************************************************************
* Assignment operator                                             *
******************************************************************/
node& node::operator = (const node rhs){
	t = rhs.t;
	i = rhs.i;
	edges = rhs.edges;

	return *this;
}

/******************************************************************
* Print operator for the node class                               *
******************************************************************/
std::ostream& operator << (std::ostream& os, const node& v) {
	os<<"Type:  "<<((v.t == VARIABLE) ? "V"/*(variable)*/ : "C"/*check*/)
	<<std::endl;
	os<<"Index: "<<v.i<<std::endl;
	os<<"Edges: "<<std::endl;

	std::list<edge>::const_iterator tmp = v.edges.begin();
	while(tmp != v.edges.end()){
		edge e = *tmp;
		node n = e.get_dest();
		os<<"|----> "<< ((n.t == VARIABLE) ?  "V" : "C" )<<n.i<<std::endl;
		++tmp;
	}
	os<<std::endl;
	
	return os;
} 

/******************************************************************
* Setters and getters                                             *
******************************************************************/
void node::set_type(node_type t){
	this->t = t;
}

void node::set_idx(int i){
	this->i = i;
}

node_type node::get_type(void) const{
	return t;
}

int node::get_idx(void) const{
	return i;
}

/******************************************************************
* Functions to add edges to the graph                             *
******************************************************************/
void node::add_edge(node& dest){
	edge e(dest);
	edges.push_back(e);		
}

void node::add_bidirectional_edge(node& dest){
	//edge e(dest);
	//edges.push_back(e);
	add_edge(dest);
	dest.add_edge(*this);
}	

/******************************************************************
* Returns a list containing all edges connected to the calling    *
* node.                                                           *
******************************************************************/
std::list<edge> node::get_edges(){
	return edges;
}

