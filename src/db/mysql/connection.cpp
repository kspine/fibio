//
//  connection.cpp
//  fibio
//
//  Created by Chen Xu on 15/09/11.
//  Copyright (c) 2015 0d0a.com. All rights reserved.
//

#include <fibio/fibers/future/async.hpp>
#include "driver.hpp"
#include "connection.hpp"
#include "statement.hpp"
#include "prepared_statement.hpp"
#include "database_metadata.hpp"

namespace fibio {
namespace db {
namespace mysql {

MySQL_Connection::MySQL_Connection(sql::Driver* drv,
                                   sql::Connection* impl,
                                   std::shared_ptr<fibers::foreign_thread_pool> executor)
: driver_(drv), impl_(impl), executor_(executor)
{
}

MySQL_Connection::~MySQL_Connection(){};

void MySQL_Connection::clearWarnings()
{
    (*executor_)([&]() { impl_->clearWarnings(); });
}

sql::Statement* MySQL_Connection::createStatement()
{
    sql::Statement* stmt = (*executor_)([&]() { return impl_->createStatement(); });
    return new MySQL_Statement(this, stmt, executor_);
}

void MySQL_Connection::close()
{
    (*executor_)([&]() { impl_->close(); });
}

void MySQL_Connection::commit()
{
    (*executor_)([&]() { impl_->commit(); });
}

bool MySQL_Connection::getAutoCommit()
{
    return (*executor_)([&]() { return impl_->getAutoCommit(); });
}

sql::SQLString MySQL_Connection::getCatalog()
{
    return (*executor_)([&]() { return impl_->getCatalog(); });
}

sql::Driver* MySQL_Connection::getDriver()
{
    return driver_;
}

sql::SQLString MySQL_Connection::getSchema()
{
    return (*executor_)([&]() { return impl_->getSchema(); });
}

sql::SQLString MySQL_Connection::getClientInfo()
{
    return (*executor_)([&]() { return impl_->getClientInfo(); });
}

void MySQL_Connection::getClientOption(const sql::SQLString& optionName, void* optionValue)
{
    (*executor_)([&]() { impl_->getClientOption(optionName, optionValue); });
}

sql::SQLString MySQL_Connection::getClientOption(const sql::SQLString& optionName)
{
    return (*executor_)([&]() { return impl_->getClientOption(optionName); });
}

void MySQL_Connection::deleter::operator()(MySQL_DatabaseMetaData *p)
{
    delete p;
}

sql::DatabaseMetaData* MySQL_Connection::getMetaData()
{
    if(!dbmeta_) {
        sql::DatabaseMetaData *m=(*executor_)([&]() { return impl_->getMetaData(); });
        dbmeta_.reset(new MySQL_DatabaseMetaData(this, m, executor_));
    }
    return dbmeta_.get();
}

sql::enum_transaction_isolation MySQL_Connection::getTransactionIsolation()
{
    return (*executor_)([&]() { return impl_->getTransactionIsolation(); });
}

const sql::SQLWarning* MySQL_Connection::getWarnings()
{
    return (*executor_)([&]() { return impl_->getWarnings(); });
}

bool MySQL_Connection::isClosed()
{
    return (*executor_)([&]() { return impl_->isClosed(); });
}

bool MySQL_Connection::isReadOnly()
{
    return (*executor_)([&]() { return impl_->isReadOnly(); });
}

bool MySQL_Connection::isValid()
{
    return (*executor_)([&]() { return impl_->isValid(); });
}

bool MySQL_Connection::reconnect()
{
    return (*executor_)([&]() { return impl_->reconnect(); });
}

sql::SQLString MySQL_Connection::nativeSQL(const sql::SQLString& sql)
{
    return (*executor_)([&]() { return impl_->nativeSQL(sql); });
}

sql::PreparedStatement* MySQL_Connection::prepareStatement(const sql::SQLString& sql)
{
    sql::PreparedStatement* stmt = (*executor_)([&]() { return impl_->prepareStatement(sql); });
    return new MySQL_PreparedStatement(this, stmt, executor_);
}

sql::PreparedStatement* MySQL_Connection::prepareStatement(const sql::SQLString& sql,
                                                      int autoGeneratedKeys)
{
    sql::PreparedStatement* stmt
        = (*executor_)([&]() { return impl_->prepareStatement(sql, autoGeneratedKeys); });
    return new MySQL_PreparedStatement(this, stmt, executor_);
}

sql::PreparedStatement* MySQL_Connection::prepareStatement(const sql::SQLString& sql, int* columnIndexes)
{
    sql::PreparedStatement* stmt
        = (*executor_)([&]() { return impl_->prepareStatement(sql, columnIndexes); });
    return new MySQL_PreparedStatement(this, stmt, executor_);
}

sql::PreparedStatement* MySQL_Connection::prepareStatement(const sql::SQLString& sql,
                                                      int resultSetType,
                                                      int resultSetConcurrency)
{
    sql::PreparedStatement* stmt = (*executor_)(
        [&]() { return impl_->prepareStatement(sql, resultSetType, resultSetConcurrency); });
    return new MySQL_PreparedStatement(this, stmt, executor_);
}

sql::PreparedStatement* MySQL_Connection::prepareStatement(const sql::SQLString& sql,
                                                      int resultSetType,
                                                      int resultSetConcurrency,
                                                      int resultSetHoldability)
{
    sql::PreparedStatement* stmt = (*executor_)([&]() {
        return impl_->prepareStatement(sql, resultSetType, resultSetConcurrency,
                                       resultSetHoldability);
    });
    return new MySQL_PreparedStatement(this, stmt, executor_);
}

sql::PreparedStatement* MySQL_Connection::prepareStatement(const sql::SQLString& sql,
                                                      sql::SQLString columnNames[])
{
    sql::PreparedStatement* stmt
        = (*executor_)([&]() { return impl_->prepareStatement(sql, columnNames); });
    return new MySQL_PreparedStatement(this, stmt, executor_);
}

void MySQL_Connection::releaseSavepoint(sql::Savepoint* savepoint)
{
    (*executor_)([&]() { impl_->releaseSavepoint(savepoint); });
}

void MySQL_Connection::rollback()
{
    (*executor_)([&]() { impl_->rollback(); });
}

void MySQL_Connection::rollback(sql::Savepoint* savepoint)
{
    (*executor_)([&]() { impl_->rollback(savepoint); });
}

void MySQL_Connection::setAutoCommit(bool autoCommit)
{
    (*executor_)([&]() { impl_->setAutoCommit(autoCommit); });
}

void MySQL_Connection::setCatalog(const sql::SQLString& catalog)
{
    (*executor_)([&]() { impl_->setCatalog(catalog); });
}

void MySQL_Connection::setSchema(const sql::SQLString& catalog)
{
    (*executor_)([&]() { impl_->setSchema(catalog); });
}

sql::Connection* MySQL_Connection::setClientOption(const sql::SQLString& optionName,
                                                   const void* optionValue)
{
    (*executor_)([&]() { impl_->setClientOption(optionName, optionValue); });
    return this;
}

sql::Connection* MySQL_Connection::setClientOption(const sql::SQLString& optionName,
                                                   const sql::SQLString& optionValue)
{
    (*executor_)([&]() { impl_->setClientOption(optionName, optionValue); });
    return this;
}

void MySQL_Connection::setHoldability(int holdability)
{
    (*executor_)([&]() { impl_->setHoldability(holdability); });
}

void MySQL_Connection::setReadOnly(bool readOnly)
{
    (*executor_)([&]() { impl_->setReadOnly(readOnly); });
}

sql::Savepoint* MySQL_Connection::setSavepoint()
{
    return (*executor_)([&]() { return impl_->setSavepoint(); });
}

sql::Savepoint* MySQL_Connection::setSavepoint(const sql::SQLString& name)
{
    return (*executor_)([&]() { return impl_->setSavepoint(name); });
}

void MySQL_Connection::setTransactionIsolation(sql::enum_transaction_isolation level)
{
    (*executor_)([&]() { impl_->setTransactionIsolation(level); });
}

} // End of namespace mysql
} // End of namespace db
} // End of namespace fibio
