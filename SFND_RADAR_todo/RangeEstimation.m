

% TODO : Find the Bsweep of chirp for 1 m resolution

c = 3*10^8; %the speed of light

delta_r = 1; %range resolution in metters 

Bsweep = c/(2*delta_r);

% TODO : Calculate the chirp time based on the Radar's Max Range
range_max = 300; % give radar max range
Ts = 5.5*(range_max *2 /c ); %5.5 times of the trip time for maximum range

% TODO : define the frequency shifts 
beat_freq = [0 1.1e6 13e6 24e6];
calculated_range = c*Ts*beat_freq/(2*Bsweep);

% Display the calculated range
disp(calculated_range);