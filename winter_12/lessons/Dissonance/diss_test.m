

% Number of Partials
n_parts = 8;
partial_offset = 1;

ints = [1 + partial_offset: n_parts + partial_offset];
amp_vec = 1 ./ (ints - partial_offset)


% f1 = 440;
% f2 = 440;

% freq from midi is 2^((n-69)/12)*440, where 69 = A4

diss = [];

o_count = 0;

for midi_1 = [47:94]
    
    o_count = o_count + 1;
    
    i_count = 0;
    
    for midi_2 = [47:94]
        
        i_count = i_count + 1;
    
        f1 = power( 2, ((midi_1 - 69)/12) ) * 440
        f2 = power( 2, ((midi_2 - 69)/12) ) * 440;
    
        part_vec_a = f1 .* ints;
        part_vec_b = f2 .* ints;

        diss(o_count, i_count) = dissmeasure([part_vec_a part_vec_b], [amp_vec amp_vec]);
    end
end
max(max(diss))
normal_diss = diss / max(max(diss));

% fid = fopen('Matrix.txt', 'wt');
% fprintf(fid, [repmat('%g\t', 1, size(normal_diss,2)-1) '%g\n'], normal_diss.');
% fclose(fid)

surf(normal_diss);
colorbar;

