\ENCODING=ISO-8859-1
\Problem name: IloCplex

Maximize
 Max_Cycle_Covers_Objective: s_0_1 + s_1_0 + s_4_0 + s_1_2 + s_2_1 + s_2_3
                             + s_3_2 + s_3_4
Subject To
 Conservation_0: s_0_1 - s_1_0 - s_4_0  = 0
 Capacity_0:     s_0_1 <= 1
 Conservation_1: - s_0_1 + s_1_0 + s_1_2 - s_2_1  = 0
 Capacity_1:     s_1_0 + s_1_2 <= 1
 Conservation_2: - s_1_2 + s_2_1 + s_2_3 - s_3_2  = 0
 Capacity_2:     s_2_1 + s_2_3 <= 1
 Conservation_3: - s_2_3 + s_3_2 + s_3_4  = 0
 Capacity_3:     s_3_2 + s_3_4 <= 1
 Conservation_4: s_4_0 - s_3_4  = 0
 Capacity_4:     s_4_0 <= 1
Bounds
 0 <= s_0_1 <= 1
 0 <= s_1_0 <= 1
 0 <= s_4_0 <= 1
 0 <= s_1_2 <= 1
 0 <= s_2_1 <= 1
 0 <= s_2_3 <= 1
 0 <= s_3_2 <= 1
 0 <= s_3_4 <= 1
End
