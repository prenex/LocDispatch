/**
 * Intersting tricks with templates and a small homebrew "type-system". Useful stuff for compile-time magic and dispatch.
 *
 * I call this stuff "compile location based dispatch" or "LocDispatch" until someone suggests a better name.
 * Basically we can write methods that act differently if the parameter came from "us" and differently when came from "others".
 * All of this is happening in compile time, so it makes stuff extremely fast when we know we can provide cached data for example...
 *
 * Compile with: g++ --std=c++14 loc_dispatch.cpp -o loc_dispatch
 * Author: Thier Richard István, u9vata@gmail.com
 */

#include <utility> /* Needed for index_sequence and stuff like that */
#include <cstdio>  /* Only here for our printf :-) */

// -------------------------------------- //
// NODE and make_node implementation here //
// -------------------------------------- //

// TODO: The special instantiation where N==0 might considered to be the "external" generic node for any tree - for data copy if needed...
template <int N, char ... Chars>
struct Node {
	int data;
};

template <int N, typename Str, std::size_t ... indices>
decltype(auto) build_node(std::index_sequence<indices...>) {
	// Rem.: for our simple example we just fill in bogus data here!
        return Node<N, Str().chars[indices]...> {N};
}

#define make_node(N, str) []{\
        struct Str { const char * chars = str; };\
        return build_node<N, Str>(std::make_index_sequence<sizeof(str)>());\
}()

// Small "test" code to see whats happening
auto hello = make_node(__LINE__, "hello");
auto test = make_node(__LINE__, __FILE__);
auto haxtest = make_node(33, __FILE__);

/*
// ------------------------------------- //
// TODO: Finish Tree implementation here //
// ------------------------------------- //
// Rem.: The tree should be tricked with the same above boring stuff to "accept" string literals in order to try this below

using ExtNode = Node<0, '.'>;

// This OWNS the memory for its nodes for whatever caching reason!
// Because of that adding a new node can be optimized if it is 
// coming from ourselves so we might optimize that and make it
// NECESSARY to give a node from our tree to see we do not copy!
template <int N, char ... Chars>
class Tree
{
	// Rem.: I simplified implementation here but you can imagine...
	Node<N, Chars...> getNode(int iteratorHandle) {
		// TODO: we just test compilation and not real stuff here
		return nullptr;
	}
	// This one always makes a copy - nevertheless if we share the tree
	// Rem.: I simplified implementation here but you can imagine...
	ExtNode getExternalNode(int iteratorHandle) {
		// TODO: we just test compilation and not real stuff here
		return nullptr;
	}

	void addNode(Node<N, Chars...> toAdd) {
		// TODO: can just copy pointers to shared data
		printf("OPTIMIZED CODE!\n");
	}

	void addNode(ExtNode toAdd) {
		printf("NON-OPTIMIZED CODE!\n");
	}
};

#define make_tree Tree< __LINE__ , __FILE__ >{}
*/

// ----------- //
// ENTRY POINT //
// ----------- //


int main() {

// Compile this with: g++ --std=c++14 loc_dispatch.cpp -o loc_dispatch
// and then you should get the following error messages for the below things if you uncommnt them:
	// OK - Should work:
	haxtest = test;
	// ERR - cannot convert ‘String<33, 't', 'e', 's', 't', '_', 's', 't', 'r', '.', 'c', 'p', 'p', '\000'>’ to ‘int’ in initialization
	int b = test;
	// ERR - cannot convert ‘String<16, 'h', 'e', 'l', 'l', 'o', '\000'>’ to ‘int’ in initialization
	int a = hello;
	// ERR - no '=' operator between the two versions
	test = hello;
/*
// Also see what is happening in here and that optimization takes place
	//auto t1 = make_tree;
	auto t1 = Tree< __LINE__ , __FILE__ >{};
	auto t2 = make_tree;

	// get root nodes with "our" type for the creation point AND for external communication too.
	auto n1 = t1.getNode(0);
	auto en1 = t1.getExternalNode(0);
	auto n2 = t1.getExternalNode(0);
	t2.addNode(en1);
	t2.addNode(n1);
	t2.addNode(n2);
*/
	return 0;
}
