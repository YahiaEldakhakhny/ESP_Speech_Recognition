% Function to add a new row to a matrix even if the size does not match by
% adding zero padding where padding is needed
function outputMatrix = addRow(inputMatrix, row)
    % Get the number of rows and columns of the inputMatrix
    [inputMatrixRowNum, inputMatrixColumnNum] = size(inputMatrix);
    % Get size of row that is to be added to the matrix
    rowSize = size(row);
    rowSize = rowSize(2);
    
    % If the inputMatrix is empty just return the row
    if inputMatrixRowNum == 0
        outputMatrix = row;
    elseif inputMatrixColumnNum > rowSize
        % Add zero padding to the row to match the size of the matrix row
        newRow = [row zeros(1, inputMatrixColumnNum - rowSize)];
        outputMatrix = [inputMatrix; newRow];
    elseif rowSize > inputMatrixColumnNum
        % Add zero padding to the rows of the matrix to match the size of
        % the new row
        newMatrix = zeros(inputMatrixRowNum, rowSize);
        for k = 1:inputMatrixRowNum
            newMatrix(k, 1:inputMatrixColumnNum) = inputMatrix(k, :);
        end
        outputMatrix = [newMatrix; row];
    else
        outputMatrix = [inputMatrix; row];
    
    % END OF IF
    end
    
    
    
% END OF FUNCTION
end