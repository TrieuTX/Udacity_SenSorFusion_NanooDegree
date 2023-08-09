% Doppler Velocity Calculation
c = 3*10^8;         %speed of light
frequency = 77e9;   %frequency in Hz

% TODO: Calculate the wavelength
Lambda = c/frequency;

% TODO: Define the doppler shifts in Hz using the information from above 
fd = [3e6 -4.5e6 11e6 -3e6];

% TODO: Calculate the velocity of the targets  fd = 2*vr/lambda
vr = (Lambda*fd)/2


% TODO: Display results
disp(vr);