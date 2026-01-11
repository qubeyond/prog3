DROP TABLE IF EXISTS ClassificationResults;
DROP TABLE IF EXISTS FeatureSets;
DROP TABLE IF EXISTS MLModels;
DROP TABLE IF EXISTS Files;
DROP TABLE IF EXISTS FileTypes;

CREATE TABLE FileTypes (
    id SERIAL PRIMARY KEY,
    extension VARCHAR(10) UNIQUE NOT NULL,
    description TEXT
);

CREATE TABLE Files (
    id SERIAL PRIMARY KEY,
    filename TEXT NOT NULL,
    size_bytes BIGINT CHECK (size_bytes > 0),
    type_id INT REFERENCES FileTypes(id) ON DELETE CASCADE
);

CREATE TABLE MLModels (
    id SERIAL PRIMARY KEY,
    model_name VARCHAR(100) NOT NULL,
    version VARCHAR(20) NOT NULL,
    algorithm TEXT
);

CREATE TABLE FeatureSets (
    id SERIAL PRIMARY KEY,
    file_id INT UNIQUE REFERENCES Files(id) ON DELETE CASCADE,
    feature_vector JSONB NOT NULL
);

CREATE TABLE ClassificationResults (
    id SERIAL PRIMARY KEY,
    file_id INT REFERENCES Files(id) ON DELETE CASCADE,
    model_id INT REFERENCES MLModels(id) ON DELETE CASCADE,
    label VARCHAR(50) NOT NULL,
    confidence NUMERIC(5, 4) CHECK (confidence >= 0 AND confidence <= 1)
);

INSERT INTO FileTypes (extension, description) VALUES 
('.exe', 'Windows Executable'),
('.pdf', 'Adobe Document'),
('.elf', 'Linux Binary'),
('.docx', 'Word Document'),
('.sh', 'Shell Script');

INSERT INTO Files (filename, size_bytes, type_id) VALUES 
('calc.exe', 102400, 1),
('report.pdf', 2048000, 2),
('malware.exe', 51200, 1),
('server.elf', 890000, 3),
('budget.docx', 45000, 4),
('install.sh', 1200, 5),
('virus.elf', 75000, 3),
('data_leak.pdf', 3500000, 2),
('explorer.exe', 450000, 1);

INSERT INTO MLModels (model_name, version, algorithm) VALUES 
('RandomForestScanner', '1.0.2', 'Random Forest'),
('RandomForestScanner', '2.0.0', 'Random Forest'), 
('NeuralSlayer', '2.1.0', 'CNN'),
('HeuristicAnalyzer', '0.5b', 'Heuristics');

INSERT INTO FeatureSets (file_id, feature_vector) VALUES 
(1, '[0.1, 0.5, 0.9]'),
(2, '[0.0, 0.2, 0.1]'),
(3, '[0.9, 0.9, 0.8]'),
(4, '[0.4, 0.1, 0.3]');

INSERT INTO ClassificationResults (file_id, model_id, label, confidence) VALUES 
(1, 1, 'Safe', 0.9850), (1, 4, 'Safe', 0.8800),
(2, 1, 'Safe', 0.9999),
(3, 1, 'Malware', 0.8700), (3, 3, 'Malware', 0.9500), (3, 4, 'Malware', 0.9900),
(4, 3, 'Safe', 0.9200),
(7, 3, 'Malware', 0.8900), (7, 4, 'Suspicious', 0.7500),
(8, 1, 'Safe', 0.9500), (8, 4, 'Malware', 0.6500); 