clear all;
close all;
clc;

% Load data from files
%% Data from play.h
file_p = load('parallel_time.dat');
rows = file_p(:,1); cols = file_p(:,2); time = file_p(:,4);
steps = file_p(:,3);

figure;
plot(steps, time);
title('Game progression');
xlabel('Iterations [-]'); ylabel('Time [s]');
grid on;

%% Grid size analysis
file_p_size = load('parallel_size_time.dat'); file_s_size = load('serial_size_time.dat');
size_size = file_p_size(:,1); time_p_size = file_p_size(:,4);
time_s_size = file_s_size(:,4);
steps_size = file_p_size(:,3);

figure;
plot(size_size, time_p_size, size_size, time_s_size);
title('Grid Size Performance Analysis');
xlabel('N for NxN Grid [-]'); ylabel('Time [s]');
legend('Parallel', 'Serial');
grid on;

figure
plot(size_size, time_s_size./time_p_size);
title('Grid Size Performance Analysis');
xlabel('N for NxN Grid [-]'); ylabel('Serial:Parallel Time Ratio [-]');
grid on;

%% Iterations analysis
file_p_its = load('parallel_its_time.dat'); file_s_its = load('serial_its_time.dat');
size_its = file_p_its(:,1); time_p_its = file_p_its(:,4);
time_s_its = file_s_its(:,4);
steps_its = file_p_its(:,3);

figure;
plot(steps_its, time_p_its, steps_its, time_s_its);
title('Iterations Performance Analysis');
xlabel('Iterations [-]'); ylabel('Time [s]');
legend('Parallel', 'Serial');
grid on;

figure
plot(steps_its, time_s_its./time_p_its);
title('Iterations Performance Analysis');
xlabel('Iterations [-]'); ylabel('Serial:Parallel Time Ratio [-]');
% ylim([5, 6.5]);
grid on;

%% No. of cores analysis
file_cores = load('parallel_cores_time.dat');
size_cores = file_cores(:,1); time_cores = file_cores(:,4);
cores = file_cores(:,3);

figure;
plot(cores, time_cores);
title('No. of Cores Analysis');
xlabel('No. of Cores [-]'); ylabel('Time [s]');
grid on;

%% File-writing analysis
file_p_FW = load('parallel_FW_time.dat'); file_s_FW = load('serial_FW_time.dat');
size_FW = file_p_FW(:,1); time_p_FW = file_p_FW(:,4);
time_s_FW = file_s_FW(:,4);
steps_FW = file_p_FW(:,3);

figure;
plot(steps_FW, time_p_FW, steps_FW, time_s_FW);
title('File-Writing Performance Analysis');
xlabel('Iterations [-]'); ylabel('Time [s]');
legend('Parallel', 'Serial');
grid on;

figure
plot(steps_FW, time_s_FW./time_p_FW);
title('File-Writing Performance Analysis');
xlabel('Iterations [-]'); ylabel('Serial:Parallel Time Ratio [-]');
grid on;

%% Image-printing analysis
file_p_IP = load('parallel_IP_time.dat'); file_s_IP = load('serial_IP_time.dat');
size_IP = file_p_IP(:,1); time_p_IP = file_p_IP(:,4);
time_s_IP = file_s_IP(:,4);
steps_IP = file_p_IP(:,3);

figure;
plot(steps_IP, time_p_IP, steps_IP, time_s_IP);
title('Image-Printing Performance Analysis');
xlabel('Iterations [-]'); ylabel('Time [s]');
legend('Parallel', 'Serial');
grid on;

figure
plot(steps_IP, time_s_IP./time_p_IP);
title('Image-Printing Performance Analysis');
xlabel('Iterations [-]'); ylabel('Serial:Parallel Time Ratio [-]');
% ylim([1, 2]);
grid on;