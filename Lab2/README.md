# AI-in-the-wild

Lab 2

 # Exercise 4:
    # So DFS is the worst one giving horrible results and taking a very long time to complete 
    # Stats for DFS -
    # Path found with total cost of 216 in 0.0 seconds
    # Search nodes expanded: 362
    # Pacman emerges victorious! Score: 414
    
    # BFS is the second best it improves way better than DFS less time and better score
    # only downside is that it takes long to load
    # Stats for BFS -
    # Path found with total cost of 60 in 7.3 seconds
    # Search nodes expanded: 16689
    # Pacman emerges victorious! Score: 570

    # Best one is A* improving the BFS the difference would be improving the time and has 1 less expanded nodes searches
    # Stats for A* -
    # Path found with total cost of 60 in 0.9 seconds
    # Search nodes expanded: 16688
    # Pacman emerges victorious! Score: 570

    # Using the foodHeuristic function makes it search the nodes less 
    # Stats using this
    # Path found with total cost of 60 in 0.9 seconds
    # Search nodes expanded: 13898
    # Pacman emerges victorious! Score: 570