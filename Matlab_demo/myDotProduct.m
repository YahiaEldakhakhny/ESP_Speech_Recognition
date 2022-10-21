function product = myDotProduct(V1, V2)
    L1 = length(V1);
    L2 = length(V2);
    
    if L1 > L2
       product = dot(V1, [V2 zeros(1, L1-L2)]);
       
    elseif L2 > L1
        product = dot(V2, [V1 zeros(1, L2-L1)]);
        
    else
        product = dot(V1, V2);
    end
end