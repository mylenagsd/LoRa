% Mylena Diniz - 04/2021
% code for graphs plotting based on an excel table with LoRa transmission
% data

clc
close all
clear 

file = 'medicoesLoRa.xlsx';   % get file

air_humid = readtable(file, 'Range', 'C2:C500');   % get humidity info
hour = readtable(file, 'Range', 'J2:J500');   % get time info
min = readtable(file, 'Range', 'K2:K500');   
sec = readtable(file, 'Range', 'L2:L500');

air_humid = table2cell(air_humid);   % converts table into cell (a:1) -> (1:a)
hour = table2cell(hour);
min = table2cell(min);
sec = table2cell(sec);

for i = 1:length(air_humid)
   info1(i) = str2num(cell2mat(air_humid(i)));   % converts cell into double (to allow plotting)
end
for i = 1:length(hour)
   h(i) = str2num(cell2mat(hour(i))); 
end
for i = 1:length(min)
   m(i) = str2num(cell2mat(min(i))); 
end
for i = 1:length(sec)
   s(i) = str2num(cell2mat(sec(i))); 
end

hour = h;   % linear time
min = m/60;
sec = s/360;
H = hour + min + sec;

figure
stem(H, info1);   % plot air humidity x hour of the day
axis([11 12 80 90]);   %always check the limits 
title('Umidade do ar vs Hora do dia');

%%

packets = readtable(file, 'Range', 'A2:A500');   % get packet info
rssi = readtable(file, 'Range', 'E2:E500');   % get RSSI info
snr = readtable(file, 'Range', 'F2:F500');   % get RSSI info


packets = table2cell(packets);   % converts table into cell (a:1) -> (1:a)
rssi = table2cell(rssi);
snr = table2cell(snr);

for i = 1:length(rssi)
   info2(i) = str2num(cell2mat(rssi(i)));   % converts cell into double (to allow plotting)
end
for i = 1:length(packets)
   info3(i) = str2num(cell2mat(snr(i))); 
end
for i = 1:length(packets)
   p(i) = str2num(cell2mat(packets(i))); 
end

figure
stem(p, info1);    % plot air humidity x packet ID
axis([1 500 80 90])   %always check the limits
title('Umidade do ar vs ID do pacote');

figure
stem(p, info2);    % plot RSSI x packet ID 
axis([1 500 -50 -30])   %always check the limits
title('RSSI vs ID do pacote');

figure
stem(p, info3);    % plot SNR x packet ID
axis([1 500 5 11])   %always check the limits
title('SNR vs ID do pacote');