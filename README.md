### HW1: Basic Search ###

(1) Breadth-first search  
(2) Depth-first search  
(3) Uniform-cost search using the distance of their relationship as cost  

### HW2: Determine the next move for a player in the Reversi game using the Greedy, Minimax, and Alpha-Beta pruning algorithms with positional weight evaluation functions. ###

(1) Greedy search  
(2) Minimax search  
(3) Alpha-Beta search  

### HW3: Logic Inference ###

Given a knowledge base and a query sentence, determine if the query can be inferred from the information given in the knowledge base. Using backward-chaining to solve this problem.

Diagnosis(John,Infected)  
6  
HasSymptom(x,Diarrhea)=>LostWeight(x)  
LostWeight(x)&Diagnosis(x,LikelyInfected)=>Diagnosis(x,Infected)   
HasTraveled(x,Tiberia)&HasFever(x)=>Diagnosis(x,LikelyInfected)  
HasTraveled(John,Tiberia)  
HasFever(John)  
HasSymptom(John,Diarrhea)  
