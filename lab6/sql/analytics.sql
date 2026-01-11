SELECT 
    m.model_name || ' (v' || m.version || ')' as model_info,
    ROUND(AVG(r.confidence)::numeric, 4) as avg_conf
FROM MLModels m
JOIN ClassificationResults r ON m.id = r.model_id
JOIN Files f ON r.file_id = f.id
WHERE f.size_bytes > 0
GROUP BY m.model_name, m.version
HAVING AVG(r.confidence) > $1
ORDER BY avg_conf DESC;