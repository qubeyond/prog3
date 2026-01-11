SELECT model_name, version 
FROM MLModels 
WHERE id IN (
    SELECT model_id 
    FROM ClassificationResults 
    WHERE confidence > (SELECT AVG(confidence) FROM ClassificationResults)
);