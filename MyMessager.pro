TEMPLATE = SUBDIRS

SUBDIRS = \
    Server \
    Client

Client.subdir = Client/
Server.subdir = Server/

CONFIG += ordered
