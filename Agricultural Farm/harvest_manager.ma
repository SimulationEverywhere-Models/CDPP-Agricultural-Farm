[top]
components : queue@queue harvester@harvester

out : out_field_1 out_field_2 out_field_3 out_bushels out_type 
in : in_surface in_field in_type
Link : in_surface in_surface@queue
Link : in_field in_field@queue
Link : in_type in_type@queue

Link : out_id@queue in_id@harvester
Link : out_surface@queue in_surface@harvester
Link : out_type@queue in_type@harvester
Link : out_id@harvester in_done@queue

Link : out_field_1@queue out_field_1
Link : out_field_2@queue out_field_2
Link : out_field_3@queue out_field_3
Link : out_bushels@harvester out_bushels
Link : out_type@harvester out_type

[queue]

[harvester]
speed : 10
yield_type_1 : 150
yield_type_2 : 120
yield_type_3 : 170