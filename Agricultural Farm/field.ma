[top]
components : field@field 
out : out_water_field out_water_surface out_water_type out_sower_field out_sower_surface out_sower_type out_harvest_field out_harvest_surface out_harvest_type 
in : in_done in_stop
Link : in_done in_done@field
Link : in_stop in_stop@field
Link : out_water_field@field out_water_field
Link : out_water_surface@field out_water_surface
Link : out_water_type@field out_water_type
Link : out_sower_field@field out_sower_field
Link : out_sower_surface@field out_sower_surface
Link : out_sower_type@field out_sower_type
Link : out_harvest_field@field out_harvest_field
Link : out_harvest_surface@field out_harvest_surface
Link : out_harvest_type@field out_harvest_type

[field]
surface : 100
id : 1
type : 1
growth_rate_1 : 3
growth_rate_2 : 6
growth_rate_3 : 9
evaporation_rate_1 : 1
evaporation_rate_2 : 2
evaporation_rate_3 : 3
