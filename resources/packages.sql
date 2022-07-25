CREATE TABLE "packages" (
        "id"	INTEGER UNIQUE,
        "opcode"	TEXT,
        "pkgLen"	INTEGER DEFAULT 0,
        "data"	TEXT,
        "timestamp"	INTEGER,
        PRIMARY KEY("id" AUTOINCREMENT)
);
