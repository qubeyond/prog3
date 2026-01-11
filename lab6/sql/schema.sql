CREATE TABLE IF NOT EXISTS FileTypes (
    id SERIAL PRIMARY KEY, 
    extension VARCHAR(10) NOT NULL, 
    description TEXT
);

CREATE TABLE IF NOT EXISTS Files (
    id SERIAL PRIMARY KEY, 
    filename TEXT NOT NULL, 
    size BIGINT CHECK (size >= 0), 
    type_id INT REFERENCES FileTypes(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS MLModels (
    id SERIAL PRIMARY KEY, 
    name TEXT NOT NULL, 
    version TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS FeatureSets (
    id SERIAL PRIMARY KEY, 
    file_id INT REFERENCES Files(id) ON DELETE CASCADE, 
    features JSONB NOT NULL
);

CREATE TABLE IF NOT EXISTS ClassificationResults (
    id SERIAL PRIMARY KEY, 
    file_id INT REFERENCES Files(id) ON DELETE CASCADE, 
    model_id INT REFERENCES MLModels(id) ON DELETE CASCADE, 
    label TEXT NOT NULL, 
    confidence FLOAT CHECK (confidence >= 0 AND confidence <= 1)
);