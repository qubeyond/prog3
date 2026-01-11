SELECT f.filename, COUNT(DISTINCT r.label) as variants
FROM Files f
JOIN ClassificationResults r ON f.id = r.file_id
GROUP BY f.filename
HAVING COUNT(DISTINCT r.label) > 1;