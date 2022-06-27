DROP TABLE IF EXISTS IMUser;
create table IMUser(
   ItemID INT NOT NULL AUTO_INCREMENT,
   LoginName VARCHAR(100) NOT NULL,
   NickName VARCHAR(100) NOT NULL,
   UserPassword VARCHAR(32) NOT NULL,
   CreateTime DATETIME,
   PRIMARY KEY ( ItemID )
);

DROP TABLE IF EXISTS UserLogin;
create table UserLogin(
    ItemID INT NOT NULL AUTO_INCREMENT,
    UserID int not null,
    LoginTime DATETIME,
	SignoutTime datetime,
    PRIMARY KEY ( ItemID ),
	CONSTRAINT UserKey FOREIGN KEY(UserID) REFERENCES IMUser(ItemID)
);