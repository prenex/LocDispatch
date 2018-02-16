# LocDispatch
Compile time dispatch based on location of the constructor to separate "us" and "them" for C++ optimization.

# How it works

	// Just see what is happening in here and that optimization takes place
	//auto t1 = make_tree;
	auto t1 = make_tree;
	auto t2 = make_tree;

	// get root nodes with "our" type for the creation point AND for external communication too.
	auto n1 = t1.getNode(0);
	auto en1 = t1.getExternalNode(0);
	auto n2 = t2.getNode(0);
	// Should print "NON-OPTIMIZED CODE!"
	t2.addNode(en1);
	// ERR - no matching function for call to ... 127 vs 128 (line number diff causes this to not compile!)
	//t2.addNode(n1); - uncomment to see error message
	// Should print "OPTIMIZED CODE!"
	t2.addNode(n2);

Basically what you see aboe is two "tree" or any kind of container that are owning the data in their nodes. For example to much better cache hits or whatever memory management reasons the nodes data might share a common place. Lets also imagine that trees are made by constructing them with RAII in places scattered around in the code - in this class and that class wherever it fits. Now these trees would easily use the extra knowledge in a more refined type system that holds information about which node are for which tree. As it turns out c++ can be "easily" extended to do this type checking and make a proper function dispatch for optimized methods that know we add a copy of a node that is already somewhere in the tree for example (so its contents are owned by us, we do not need to copy and re-create them).

Implementation is a bit tricky and found out this quite randomly from pieces scattered around the net and my already existing code, so I thought it is good to put up an example of this. In the end I might choose to not use this directly - but a changed version - however it can be of use for anyone maybe...
