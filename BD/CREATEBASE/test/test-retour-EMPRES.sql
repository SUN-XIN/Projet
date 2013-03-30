# Fichier EmpRes.sql
# Creation de la table EMPRES (EMPRUNTS/RESERVATIONS) 


DROP TABLE EMPRES;

CREATE TABLE EMPRES
( NoFilm smallint NOT NULL,
  NoExemplaire smallint NOT NULL,
  CodeAbonne varchar(8) NOT NULL REFERENCES ABONNES (Code),
  DateEmpRes datetime,
  PRIMARY KEY (NoFilm,NoExemplaire),
  FOREIGN KEY (NoFilm,NoExemplaire) REFERENCES CASSETTES
);

INSERT INTO EMPRES
VALUES(17, 1, "365AL8","2012-05-01 14:30:50");

INSERT INTO EMPRES
VALUES(17, 3, "4367Xs","2012-04-16 10:39:49");

INSERT INTO EMPRES
VALUES(18, 2, "365AL8","2012-05-02 13:09:10");