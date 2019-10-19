/****************************************************************************
**
** Copyright (C) 2019 Eric Lee.
** Contact: levanhong05@gmail.com
** Company: Freelancer
**
** This file is part of the F2A Generator project.
**
**All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
**
****************************************************************************/

#ifndef SOURCENODE_H
#define SOURCENODE_H

#include <QChar>
#include <QObject>
#include <QStack>

class SourceNode : public QObject
{
    Q_OBJECT
public:
    static const int META_NODE = 0;
    static const int FUNCTION_NODE = 1;
    static const int LOOP_NODE = 2;
    static const int SWITCH_NODE = 3;
    static const int CONDITION_NODE = 4;
    static const int DATA_NODE = 5;
    static const int PARAM_NODE = 6;
    static const int BLOCK_COMMENT = 7;
    static const int LINE_COMMENT = 8;

    SourceNode(QString name = "", QString value = "", int nt = FUNCTION_NODE);
    SourceNode(QString name, QList<SourceNode *> childs, QString value = "", int nt = FUNCTION_NODE);
    SourceNode(SourceNode *clone);

    SourceNode *create(QString name = "", QString value = "", int nt = FUNCTION_NODE);

    ~SourceNode();

    // getters
    QString name();
    QString value();
    QString inlineComment();

    int nodeType();
    int index();
    QString path();

    SourceNode *parent();
    QList<SourceNode *> childs();
    SourceNode *childAt(int index);
    bool hasChilds();
    int childsCount();

    // setters | modifiers
    void setName(QString name);
    void setValue(QString value);
    void setInlineComment(QString cm);

    void setNodeType(int nt);
    void setIndex(int index);
    void reindex();

    void setParent(SourceNode *parent);

    SourceNode *addChildNode(SourceNode *node);
    SourceNode *addChildAfter(SourceNode *node, SourceNode *childNode);
    SourceNode *addChildBefore(SourceNode *node, SourceNode *childNode);

    void removeChild(SourceNode *child);
    void removeAllChild();
    void remove();

    // reader
    void loadFromFile(QString filename);

    // parser
    virtual SourceNode *parse(QStringList *tokens, SourceNode *node = nullptr, QStack<QString> operation = QStack<QString>(), QStringList *lastTokens = nullptr);

    // writer | formater
    virtual QString saveToString();
    virtual void saveToFile(QString filename);
    virtual QStringList format(SourceNode *node = nullptr, int level = 0);

    // query utils
    SourceNode *get(QString path);
    QList<SourceNode *> query(QString q);

    bool isRootNode();
    SourceNode *rootNode();

protected:
    QString makeSpaces(int n, QChar ch = QChar(' '));
    QString makeTabs(QString name);
    QString tabsToSpaces(QString line);

signals:
    void nodeChanged(SourceNode *rootNode);

public slots:
    void sendNodeChangedSignal();

protected:
    QString _name;
    QString _value;
    QString _inlineComment;

    int _nodeType;
    int _index;

    SourceNode *_parent;
    QList<SourceNode *> _childs;
    SourceNode *lastChildNode;

};

typedef QList<SourceNode *> SourceNodes;

#endif // SOURCENODE_H
