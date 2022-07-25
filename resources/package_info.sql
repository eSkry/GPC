CREATE TABLE "package_info" (
        "id"	INTEGER,
        "opcode"	TEXT NOT NULL UNIQUE,
        "name"	TEXT,
        "color"	TEXT,
        "ingnore"	INTEGER DEFAULT 0,
        "lastTimestamp"	TEXT DEFAULT 0,
        PRIMARY KEY("id" AUTOINCREMENT)
);
