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

#include "sourcenode.h"

#include <QFile>
#include <QRegExp>
#include <QFileInfo>
#include <QTextStream>

#include <QDebug>

SourceNode::SourceNode(QString name, QString value, int nt)
{
    this->_name = name;
    this->_value = value;
    this->_nodeType = nt;
    this->_parent = nullptr;
}

SourceNode::SourceNode(QString name, QList<SourceNode *> childs, QString value, int nt)
{
    this->_name = name;
    this->_childs = childs;
    this->_value = value;
    this->_nodeType = nt;
    this->_parent = nullptr;
}

SourceNode::SourceNode(SourceNode *clone)
{
    this->_name = clone->name();
    this->_value = clone->value();
    this->_nodeType = clone->nodeType();
    this->_parent = clone->parent();
    this->_index = clone->index();
    this->_inlineComment = clone->inlineComment();
    this->_childs = QList<SourceNode *>();

    foreach (SourceNode *child, clone->childs()) {
        this->addChildNode(new SourceNode(child));
    }
}

SourceNode *SourceNode::create(QString name, QString value, int nt)
{
    SourceNode *node = new SourceNode(name, value, nt);

    return this->addChildNode(node);
}

SourceNode::~SourceNode()
{
}

QString SourceNode::name()
{
    return this->_name;
}

QString SourceNode::value()
{
    return this->_value;
}

int SourceNode::nodeType()
{
    return this->_nodeType;
}

QString SourceNode::inlineComment()
{
    return this->_inlineComment;
}

void SourceNode::setName(QString name)
{
    this->_name = name;
}

void SourceNode::setValue(QString value)
{
    this->_value = value;
}

SourceNode *SourceNode::addChildNode(SourceNode *node)
{
    if (node != nullptr) {
        this->_childs.append(node);
        node->setParent(this);
        node->setIndex(this->childsCount() + 1);
    }

    return this;
}

SourceNode *SourceNode::addChildAfter(SourceNode *node, SourceNode *childNode)
{
    if (node != nullptr && childNode != nullptr) {
        if (node->index() == this->childsCount()) {
            return this->addChildNode(childNode);
        }

        this->_childs.insert(node->index(), childNode);
        childNode->setParent(this);

        this->reindex();
    }

    return this;
}

SourceNode *SourceNode::addChildBefore(SourceNode *node, SourceNode *childNode)
{
    if (node != nullptr && childNode != nullptr) {
        this->_childs.insert(node->index() - 1, childNode);
        childNode->setParent(this);

        this->reindex();
    }

    return this;
}

void SourceNode::setParent(SourceNode *parent)
{
    this->_parent = parent;
}

void SourceNode::setIndex(int index)
{
    this->_index = index;
}

void SourceNode::setNodeType(int nt)
{
    this->_nodeType = nt;
}

void SourceNode::setInlineComment(QString cm)
{
    this->_inlineComment = cm;
}

void SourceNode::remove()
{
    this->parent()->removeChild(this);
}

void SourceNode::reindex()
{
    for (int i = 1; i <= this->_childs.count(); i++) {
        this->_childs.at(i - 1)->setIndex(i);
    }
}

void SourceNode::removeChild(SourceNode *child)
{
    this->_childs.removeOne(child);
    this->reindex();
}

void SourceNode::removeAllChild()
{
    while (this->hasChilds()) {
        this->removeChild(this->childAt(1));
    }
}

int SourceNode::childsCount()
{
    return this->childs().length();
}

int SourceNode::index()
{
    return this->_index;
}

SourceNode *SourceNode::childAt(int index)
{
    return this->_childs.at(index);
}

QString SourceNode::path()
{
    if (this->parent() != nullptr) {
        return this->parent()->path() + '/' + this->name();
    } else {
        return this->name();
    }
}

bool SourceNode::hasChilds()
{
    return this->_childs.length() > 0;
}

SourceNodes SourceNode::childs()
{
    return this->_childs;
}

SourceNode *SourceNode::parent()
{
    return this->_parent;
}

bool SourceNode::isRootNode()
{
    return _parent == nullptr;
}

SourceNode *SourceNode::rootNode()
{
    if (this->isRootNode()) {
        return this;
    } else {
        return this->_parent->rootNode();
    }
}

QString SourceNode::saveToString()
{
    return this->format(this).join("\n");
}

void SourceNode::saveToFile(QString filename)
{
    QFile outputFile(filename);
    QStringList content(this->format(this));

    if (outputFile.open(QIODevice::WriteOnly)) {
        QTextStream out(&outputFile);

        foreach (QString line, content) {
            line += "\n";
            out << line.toUtf8();
        }

        outputFile.flush();
        outputFile.close();
    }
}

void SourceNode::loadFromFile(QString filename)
{
    QFile inputFile(filename);
    QStringList contents;

    if (inputFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&inputFile);

        bool isMultiLineComment = false;

        while (!in.atEnd()) {
            QString line = in.readLine().trimmed().remove("\t");

            if (line.startsWith("//")) {
                continue;
            } else if (line.startsWith("/*")) {
                isMultiLineComment = true;
                continue;
            } else if (line.endsWith("*/")) {
                isMultiLineComment = false;
                continue;
            }

            QStringList tokens = line.split(" ", QString::SkipEmptyParts);

            if (!tokens.isEmpty() && line.contains(QRegExp("^(#include|#define|#using|#if|#elif|#else|#endif|#ifdef|#ifndef|#undef|#import|#error|#line) .+"))) {
                continue;
            }

            if (!isMultiLineComment && !line.isEmpty()) {
                contents.append(line);
            }
        }

       inputFile.close();
    }

    this->parse(&contents, this);
}

SourceNode *SourceNode::parse(QStringList *tokens, SourceNode *node, QStack<QString> operation, QStringList *lastTokens)
{
    if (node == nullptr) {
        node = new SourceNode();
        node->setName("root");
    }

    if (lastTokens == nullptr) {
        lastTokens = new QStringList();
        lastTokens->append("START");
    }

    while (!tokens->isEmpty()) {
        QString token(tokens->first());
        lastTokens->append(token);
        tokens->removeFirst();

        int size = token.count("{");

        for (int i = 0; i < size; i++) {
            operation.push("{");
        }

        size = token.count("}");

        for (int i = 0; i < size; i++) {
            operation.pop();
        }

        if (token == "{") {
            continue;
        }

        if (token == "}") {
            if (operation.isEmpty()) {
                return node;
            } else {
                continue;
            }
        }

        QStringList values = token.split(" ", QString::SkipEmptyParts);

        if (!values.isEmpty() && token.contains(QRegExp("^(void|int|bool|float|double|long) .+")) && !token.contains("=")) {
            if (token.endsWith(";")) {
                continue;
            } else {
                //Function
                bool isFunction = false;
                QString functionName = "";
                QString comment = "";

                if (!tokens->isEmpty()) {
                    QString nextToken(tokens->first());

                    if (nextToken.startsWith("{")) {
                        isFunction = true;
                    }
                }

                if (!isFunction && token.contains("{")) {
                    isFunction = true;
                }

                values.clear();

                if (token.contains("//")) {
                    values = token.split("//", QString::SkipEmptyParts);
                } else if (token.contains("/*")) {
                    values = token.split("/*", QString::SkipEmptyParts);
                }

                if (!values.isEmpty()) {
                    if (values.length() > 1) {
                        comment = values[1];
                    }

                    values = values[0].split(QRegExp("\\(|\\)"), QString::SkipEmptyParts);

                    if (values.length() > 1) {
                        values = values[0].split(" ", QString::SkipEmptyParts);

                        if (!values.isEmpty()) {
                            functionName = values.last();
                        }
                    }
                }

                if (isFunction) {
                    lastChildNode = new SourceNode();
                    lastChildNode->setName(functionName);
                    lastChildNode->setNodeType(FUNCTION_NODE);
                    lastChildNode->setInlineComment(comment);

                    lastChildNode->setParent(node);

                    lastChildNode = parse(tokens, lastChildNode, operation, lastTokens);

                    if (lastChildNode != nullptr) {
                        node->addChildNode(lastChildNode);
                    }
                }
            }
        } else {
            if (token.startsWith("struct", Qt::CaseInsensitive)) {
                //TODO
            } else if (token.contains(QRegExp("^.*(else if).+\\(.+\\).*$"))) {
                //TODO
                qDebug() << token;
            } else if (token.contains(QRegExp("^.*(if).+\\(.+\\).*$"))) {
                //TODO
                qDebug() << token;
            } else if (token.contains(QRegExp("^.*(for).+\\(.+(;).+(;).+\\).*$"))) {
                //TODO
                qDebug() << token;
            } else if (token.contains(QRegExp("^.*(while).+\\(.+(<|>|>=|<=|!|==).+\\).*$"))) {
                //TODO
                qDebug() << token;
            } else if (token.startsWith("do", Qt::CaseInsensitive)) {
                //TODO
            } else {
                if (token.contains("=") && !token.contains("==") && !token.contains(">=") && !token.contains("<=")) {
                    token = token.remove(";");

                    QStringList values = token.split("=", QString::SkipEmptyParts);

                    QString key = "";
                    QString value = "";

                    if (values.length() > 1) {
                        key = values[0].split(" ", QString::SkipEmptyParts).last().trimmed();
                        value = values[1];
                    } else if (values.length() > 0) {
                        key = values[0];
                    }

                    SourceNode *child = new SourceNode(key, value);
                    child->setNodeType(DATA_NODE);

                    node->addChildNode(child);
                } else {
                    token = token.remove(";");
                    SourceNode *child = new SourceNode(token, "");
                    child->setNodeType(DATA_NODE);

                    node->addChildNode(child);
                }
            }

            if (operation.isEmpty()) {
                return node;
            }
        }

        if (tokens->isEmpty()) {
            return node;
        }
    }

    return nullptr;
}

SourceNode *SourceNode::get(QString path)
{
    QStringList paths = path.split('/', QString::SkipEmptyParts);
    int total = paths.length();

    SourceNode *node = this;

    for (int i = 0; i < total; ++i) {
        int count = 0;

        foreach (SourceNode *child, node->childs()) {
            count++;

            if (child->name().toLower() == paths.at(i).toLower()) {
                if (i == total - 1) {
                    return child;
                } else {
                    node = child;
                    break;
                }
            } else {
                if (count == node->childsCount()) {
                    return nullptr;
                }
            }
        }
    }

    return nullptr;
}

QList<SourceNode *> SourceNode::query(QString q)
{
    QStringList paths = q.split('/', QString::SkipEmptyParts);
    QList<SourceNode *> results;

    foreach (SourceNode *child, this->childs()) {
        QRegExp regexp(paths[0]);
        regexp.setPatternSyntax(QRegExp::Wildcard);
        int pos = regexp.indexIn(child->name());

        if (pos == 0) {
            if (paths.length() > 1) {
                results.append(child->query(((QStringList)paths.mid(1)).join("/")));
            } else {
                results.append(child);
            }
        }
    }

    return results;
}

QString SourceNode::makeSpaces(int n, QChar ch)
{
    QString result("");

    for (int i = 0; i < n; ++i) {
        result.append(ch);
    }

    return result;
}

QString SourceNode::makeTabs(QString name)
{
    int length = name.length();
    int count = 6;

    if (length > 20) {
        count = 1;
    } else if (length >= 16) {
        count = 2;
    } else if (length >= 12) {
        count = 3;
    } else if (length >= 8) {
        count = 4;
    } else if (length >= 4) {
        count = 5;
    }

    return this->makeSpaces(count, '\t');
}

QString SourceNode::tabsToSpaces(QString line)
{
    QString seperator("");
    int tabsCount = line.count('\t');
    line = line.replace(QRegExp("\\t+"), "\t");

    QStringList parts = line.split('\t');
    QString name = parts.at(0).trimmed();

    // first tab exception
    int reduceSpaces = 4 - name.length() % 4;
    seperator.append(this->makeSpaces(reduceSpaces, ' '));
    tabsCount--;

    seperator.append(this->makeSpaces(tabsCount * 4, ' '));
    line = line.replace("\t", seperator);

    return line;
}

void SourceNode::sendNodeChangedSignal()
{
    if (isRootNode()) {
        emit nodeChanged(this);
    } else {
        this->rootNode()->sendNodeChangedSignal();
    }
}

QStringList SourceNode::format(SourceNode *node, int level)
{
    QStringList result;

    if (node == nullptr) {
        node = this;
    }

    foreach (SourceNode *child, node->childs()) {
        QString inlineComment = "";

        if (!child->inlineComment().isEmpty()) {
            inlineComment = " " + child->inlineComment();
        }

        if (!child->hasChilds()) {
            if (child->nodeType() == FUNCTION_NODE) {
                if (child->value().length() == 0) {
                    result.append(this->tabsToSpaces(this->makeSpaces(level * 4) +
                                                     child->name() + "\n"+
                                                     this->makeSpaces(level * 4) + "{\n\n" +
                                                     this->makeSpaces(level * 4) + "} ") + inlineComment);
                } else {
                    result.append(this->tabsToSpaces(this->makeSpaces(level * 4) +
                                                 child->name() +
                                                 (child->value().length() == 0
                                                  ? ""
                                                  : this->makeTabs(child->name())) +
                                                 child->value() + "; ") + inlineComment);
                }
            } else { // is block comment
                result.append(this->makeSpaces(level * 4) + child->value());
            }

            if (level == 0) {
                result.append("");
            }
        } else {
            result.append(this->makeSpaces(level * 4) + child->name());
            result.append(this->makeSpaces(level * 4) + "{");
            result.append(this->format(child, level + 1));
            result.append(this->makeSpaces(level * 4) + "} " + inlineComment);
            result.append("");
        }
    }

    return result;
}
