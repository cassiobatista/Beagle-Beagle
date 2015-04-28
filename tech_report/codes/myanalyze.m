% Apr 2015: Cassio Trindade Batista
% myanalyze: Converts vectors of durations into step functions and plot'em all

function y = myanalyze(a, b, c, d)

debug = false;

is_vec = [~isvector(a) ~isvector(b) ~isvector(c) ~isvector(d)];
min_len = [length(a)<35 length(b)<35 length(c)<35 length(d)<35];

if sum([sum(is_vec) sum(min_len)]) ~= 0
	error('TA ERRADO FILHOS DAS PUAS')
end

% threshold: expecting specify a enough range to get the entire signal
myend = 43;

% inside the range, extracts exactly the signal that matters without repeating
[m, id] = max(a(2:myend));
a = a(2:id);

[m, id] = max(b(2:myend));
b = b(2:id);

[m, id] = max(c(2:myend));
c = c(2:id);

[m, id] = max(d(2:myend));
d = d(2:id);

if debug
	figure;
	subplot(4,1,1);
	plot(a, 'b', 'LineWidth', 3);
	xlim([0 myend]); set(gca, 'XTick', [0:1:myend]); grid on;
	subplot(4,1,2);
	plot(b, 'r', 'LineWidth', 3);
	xlim([0 myend]); set(gca, 'XTick', [0:1:myend]); grid on;
	subplot(4,1,3);
	plot(c, 'g', 'LineWidth', 3);
	xlim([0 myend]); set(gca, 'XTick', [0:1:myend]); grid on;
	subplot(4,1,4);
	plot(d, 'k', 'LineWidth', 3);
	xlim([0 myend]); set(gca, 'XTick', [0:1:myend]); grid on;
end

fprintf('dur_a_ms = %.3f \n', sum(a)/1000);
fprintf('dur_b_ms = %.3f \n', sum(b)/1000);
fprintf('dur_c_ms = %.3f \n', sum(c)/1000);
fprintf('dur_d_ms = %.3f \n', sum(d)/1000);

y = mean([sum(a) sum(b) sum(c) sum(d)])/1000;

% Periodo da informação completa (14 bits) RC-5
% 14 bits * 64 pulsos por bit * periodo de uma piscada do led
%duration_T_38k = 14 * 2*32*(1/38000) * 1000

% Converting from vector of durations into step function
% if a piece lasts n usecs then it'll become a step with n ones or zeros
x_a = x_b = x_c = x_d = [];
for i=1:length(a)
	if rem(i, 2) == 1 % odd .:. crista
		x_a = [x_a ones(1, a(i))];
	else
		x_a = [x_a zeros(1, a(i))];
	end
end

for i=1:length(b)
	if rem(i, 2) == 1 % odd .:. crista
		x_b = [x_b ones(1, b(i))];
	else
		x_b = [x_b zeros(1, b(i))];
	end
end

for i=1:length(c)
	if rem(i, 2) == 1 % odd .:. crista
		x_c = [x_c ones(1, c(i))];
	else
		x_c = [x_c zeros(1, c(i))];
	end
end

for i=1:length(d)
	if rem(i, 2) == 1 % odd .:. crista
		x_d = [x_d ones(1, d(i))];
	else
		x_d = [x_d zeros(1, d(i))];
	end
end

myend=23000;
figure;
subplot(4,1,1);
plot(x_a, 'b', 'LineWidth', 5);
xlim([0 myend]); set(gca, 'XTick', [0:2000:myend]); grid on; set(gca, 'YTick', [0:1]);
subplot(4,1,2);
plot(x_b, 'r', 'LineWidth', 5);
xlim([0 myend]); set(gca, 'XTick', [0:2000:myend]); grid on; set(gca, 'YTick', [0:1]);
subplot(4,1,3);
plot(x_c, 'g', 'LineWidth', 5);
xlim([0 myend]); set(gca, 'XTick', [0:2000:myend]); grid on; set(gca, 'YTick', [0:1]);
subplot(4,1,4);
plot(x_d, 'k', 'LineWidth', 5);
xlim([0 myend]); set(gca, 'XTick', [0:2000:myend]); grid on; set(gca, 'YTick', [0:1]);

%%% EOF %%%
