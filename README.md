# google-maps-mirroring
This project makes use of dijkstra's shortest path algorithm to find the shortest path between two cities.
All you need to run this project is gcc compiler. To run this project, create an
object code

$ gcc helper.c update_graph.c shortest_path.c client.c -o main

After creating the object code, run the object code and give the input as specified by the program

$ ./main

Do make use of the file city.txt which stores the names of all the cities in the graph considered.

Also there is a report regarding the whole project. One might take a look into that for the various decisions and design issues
encountered
