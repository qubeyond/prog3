SELECT f.filename, t.extension, f.size_bytes
FROM Files f 
JOIN FileTypes t ON f.type_id = t.id 
WHERE t.extension = '.exe';