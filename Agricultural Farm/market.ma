[top]
components : market@market 
out : out_type out_bushels out_sales
in : in_bushels in_type 
Link : in_bushels in_bushels@market
Link : in_type in_type@market
Link : out_type@market out_type
Link : out_bushels@market out_bushels
Link : out_sales@market out_sales

[market]
value_type_1 : 2.5
value_type_2 : 3.2
value_type_3 : 4.1