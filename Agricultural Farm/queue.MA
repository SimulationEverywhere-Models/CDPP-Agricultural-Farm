[top]
components : queue@queue 
out : out_id out_surface out_type out_field_1 out_field_2 out_field_3 
in : in_done in_surface in_field in_type 
Link : in_done in_done@queue
Link : in_surface in_surface@queue
Link : in_field in_field@queue
Link : in_type in_type@queue
Link : out_id@queue out_id
Link : out_surface@queue out_surface
Link : out_type@queue out_type
Link : out_field_1@queue out_field_1
Link : out_field_2@queue out_field_2
Link : out_field_3@queue out_field_3

[queue]


