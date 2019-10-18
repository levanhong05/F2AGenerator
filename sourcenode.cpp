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

SourceNode::SourceNode(QString name, QString value, int nt)
{
    this->_name = name;
    this->_value = value;
    this->_nodeType = nt;
    this->_parent = NULL;
}

SourceNode::SourceNode(QString name, QList<SourceNode *> childs, QString value, int nt)
{
    this->_name = name;
    this->_childs = childs;
    this->_value = value;
    this->_nodeType = nt;
    this->_parent = NULL;
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

    return this == NULL ? node : this->addChildNode(node);
}

SourceNode::~SourceNode()
{
}

QString SourceNode::name()
{
    return this == NULL ? "" : this->_name;
}

QString SourceNode::value()
{
    return this == NULL ? "" : this->_value;
}

int SourceNode::nodeType()
{
    return this == NULL ? 0 : this->_nodeType;
}

QString SourceNode::inlineComment()
{
    return this == NULL ? "" : this->_inlineComment;
}

void SourceNode::setName(QString name)
{
    if (this == NULL) {
        return;
    }

    this->_name = name;
}

void SourceNode::setValue(QString value)
{
    if (this == NULL) {
        return;
    }

    this->_value = value;
}

SourceNode *SourceNode::addChildNode(SourceNode *node)
{
    if (node != 0) {
        this->_childs.append(node);
        node->setParent(this);
        node->setIndex(this->childsCount() + 1);
    }

    return this;
}

SourceNode *SourceNode::addChildAfter(SourceNode *node, SourceNode *childNode)
{
    if (node != 0 && childNode != 0) {
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
    if (node != 0 && childNode != 0) {
        this->_childs.insert(node->index() - 1, childNode);
        childNode->setParent(this);

        this->reindex();
    }

    return this;
}

void SourceNode::setParent(SourceNode *parent)
{
    if (this == NULL) {
        return;
    }

    this->_parent = parent;
}

void SourceNode::setIndex(int index)
{
    if (this == NULL) {
        return;
    }

    this->_index = index;
}

void SourceNode::setNodeType(int nt)
{
    if (this == NULL) {
        return;
    }

    this->_nodeType = nt;
}

void SourceNode::setInlineComment(QString cm)
{
    if (this == NULL) {
        return;
    }

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
    return this == NULL ? 0 : this->childs().length();
}

int SourceNode::index()
{
    return this == NULL ? 0 : this->_index;
}

SourceNode *SourceNode::childAt(int index)
{
    return this == NULL ? NULL : this->_childs.at(index - 1);
}

QString SourceNode::path()
{
    if (this->parent() != NULL) {
        return this->parent()->path() + '/' + this->name();
    } else {
        return this->name();
    }
}

bool SourceNode::hasChilds()
{
    return this == NULL ? false : this->_childs.length() > 0;
}

SourceNodes SourceNode::childs()
{
    return this == NULL ? QList<SourceNode*>() : this->_childs;
}

SourceNode *SourceNode::parent()
{
    return this == NULL ? NULL : this->_parent;
}

bool SourceNode::isRootNode()
{
    return this == NULL ? true : _parent == NULL;
}

SourceNode *SourceNode::rootNode()
{
    if (this->isRootNode()) {
        return this;
    } else {
        return this->_parent->rootNode();
    }
}

void SourceNode::loadFromFile(QString filename)
{
    QFile inputFile(filename);
    QString content("");

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
            } else if (line.startsWith("*/")) {
                isMultiLineComment = false;
                continue;
            }

            if (!isMultiLineComment) {
                content.append(line + "\n");
            }
        }

       inputFile.close();
    }

    this->loadFromString(content);
}

void SourceNode::loadFromString(QString sourceContent)
{
    QRegExp inlineCommentExpr("//.*");
    QRegExp multiLinesCommentExpr("/\\*.*");
    QRegExp endMultiLinesCommentExpr("\\*/");
    QRegExp sepExpr("\\s+");

    QTextStream stream(new QString(sourceContent));
    QStringList tokens;
    bool inBlockComment = false;
    QString line = stream.readLine();

    while (!line.isNull()) {
        if (inBlockComment) {
            int endBcommentPos = endMultiLinesCommentExpr.indexIn(line, 0);

            if (endBcommentPos >= 0) {
                endBcommentPos += 2;
                QString partialBlockComment = line.left(endBcommentPos);
                inBlockComment = false;
                tokens.last().append("\n" + partialBlockComment);
                line = line.right(line.length() - partialBlockComment.length());
                line.trimmed();

                if (line.length() == 0) {
                    line = stream.readLine();
                }

                continue;
            } else {
                tokens.last().append("\n" + line);
            }
        } else {
            QString lineComment;
            QString blockComment;

            if (line.startsWith("&")) {
                tokens.append(line);
                line = stream.readLine();
                continue;
            }

            int commentPos = inlineCommentExpr.indexIn(line, 0);

            if (commentPos >= 0) {
                lineComment = line.mid(commentPos);
                line.chop(lineComment.length());
                line = line.trimmed();

                if (line.length() == 0) {
                    lineComment = "\n" + lineComment;
                }
            }

            int blockCommentPos = multiLinesCommentExpr.indexIn(line, 0);

            if (blockCommentPos >= 0) {
                if (lineComment.length() > 0) {
                    line += lineComment;
                }

                blockComment = line.mid(blockCommentPos);
                line = line.mid(0, blockCommentPos);
                inBlockComment = true;
            }

            if (line.length() > 0) {
                tokens.append(line.split(sepExpr, QString::SkipEmptyParts));
            }

            if (inBlockComment) {
                tokens.append(blockComment);
            }

            if (!inBlockComment && commentPos >= 0) {
                tokens.append(lineComment);
            }
        }

        line = stream.readLine();
    }

    this->parse(&tokens, this);
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

SourceNode *SourceNode::parse(QStringList *tokens, SourceNode *node, QStringList *lastTokens)
{
    if (node == 0) {
        node = new SourceNode();
    }

    QStringList values;
    QRegExp parentheseRegexp(".+\\(.+");

    if (lastTokens == 0) {
        lastTokens = new QStringList();
        lastTokens->append("START");
    }

    while (!tokens->isEmpty()) {
        QString token(tokens->first());
        lastTokens->append(token);
        QString lastToken = lastTokens->at(lastTokens->count() - 2);
        tokens->removeFirst();
        values.append(token);

        if (token.startsWith("\n//")) {
            SourceNode *lineComment = new SourceNode();
            lineComment->setNodeType(LINE_COMMENT);
            lineComment->setValue(token.mid(1));
            node->addChildNode(lineComment);
            values.clear();
            continue;
        }

        if (token.startsWith("/*")) {
            SourceNode *blockComment = new SourceNode();
            blockComment->setNodeType(BLOCK_COMMENT);
            blockComment->setValue(token);
            node->addChildNode(blockComment);
            values.clear();
            continue;
        }

        if (token.startsWith("//")) {
            QString inlineComment = token;

            if (lastChildNode != 0) {
                lastChildNode->setInlineComment(inlineComment);
            }

            values.removeLast();
            continue;
        }

        if (token.startsWith("!")) {
            values.removeLast();
            values.append(token.mid(1));
            continue;
        }

        if (token.startsWith("&")) {
            values.removeLast();
            values.append(token.mid(1));
            continue;
        }

        if (token.contains(")(") && !lastToken.endsWith("}") && !lastToken.endsWith(";")) {
            QString replace = token.replace(")(", ") (");
            QStringList elements =
                replace.split(QRegExp("\\s+"), QString::SkipEmptyParts);

            for (int i = elements.length() - 1; i >= 0; i--) {
                tokens->prepend(elements.at(i));
            }

            values.removeLast();
            continue;
        }

        if (parentheseRegexp.exactMatch(token) && token.length() > 1 && !lastToken.endsWith("}") && !lastToken.endsWith(";")) {
            QStringList parts = token.split('(');

            if (parts.length() > 1) {
                tokens->prepend(parts.at(1));
            }

            tokens->prepend("(");
            tokens->prepend(parts.at(0));
            values.removeLast();
            continue;
        }

        if (token.startsWith("{")) {
            lastChildNode = new SourceNode();

            if (values.length() > 0 && values.first().length() > 0) {
                values.removeLast();

                if (values.length() > 0) {
                    lastChildNode->setName(values.join(" "));
                }
            } else {
                lastChildNode->setName("");
            }

            values.clear();

            if (token.length() > 1) {
                token.remove(0, 1);
                tokens->prepend(token);
            }

            lastChildNode->setParent(node);
            lastChildNode = parse(tokens, lastChildNode, lastTokens);

            if (lastChildNode != NULL) {
                node->addChildNode(lastChildNode);
            }
        }

        if (token.endsWith('}')) {
            if (token.length() > 1) {
                token.chop(1);

                tokens->append(token);
                tokens->append("}");

                values.removeLast();
            } else {
                if (values.length() > 1) {
                    for (int i = 0; i < values.length() - 1; i++) {
                        SourceNode *child = new SourceNode(values[i]);
                        node->addChildNode(child);
                    }
                }
                return node;
            }
        }

        if (token.endsWith(';')) {
            if (token.length() > 1) {
                token.chop(1);
                tokens->prepend(";");
                tokens->prepend(token);
                values.removeLast();
            } else {
                lastChildNode = new SourceNode();
                lastChildNode->setName(values.first());
                values.removeFirst();

                if (!values.empty()) {
                    values.removeLast();

                    if (!values.empty()) {
                        lastChildNode->setValue(values.join(" "));
                        values.clear();
                    }
                }

                node->addChildNode(lastChildNode);
            }
        }
    }

    return 0;
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
                    return NULL;
                }
            }
        }
    }

    return NULL;
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

    if (node == 0) {
        node = this;
    }

    foreach (SourceNode *child, node->childs()) {
        QString inlineComment = "";

        if (!child->inlineComment().isEmpty()) {
            inlineComment = " " + child->inlineComment();
        }

        if (!child->hasChilds()) {
            if (child->nodeType() == DATA_NODE) {
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
