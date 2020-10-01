[top]
components : harvester@harvester 
out : out_id out_bushels out_type 
in : in_id in_surface in_type 
Link : in_id in_id@harvester
Link : in_surface in_surface@harvester
Link : in_type in_type@harvester
Link : out_id@harvester out_id
Link : out_bushels@harvester out_bushels
Link : out_type@harvester out_type

[harvester]
speed : 10
yield_type_1 : 150
yield_type_2 : 120
yield_type_3 : 170


