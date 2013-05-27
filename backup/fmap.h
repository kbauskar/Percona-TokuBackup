/* -*- mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*- */
// vim: ft=cpp:expandtab:ts=8:sw=4:softtabstop=4:
#ifndef FMAP_H
#define FMAP_H

#ident "Copyright (c) 2012-2013 Tokutek Inc.  All rights reserved."
#ident "$Id$"


#include <vector>
#include "description.h"
#include "backup_directory.h"

class backup_directory;

class fmap
{
private:
    std::vector<description *> m_map;
public:
    fmap() throw();
    ~fmap() throw();

    void get(int fd, description**result) throw();
    // Effect:   Returns pointer (in *result) to the file description object that matches the
    //   given file descriptor.  This will return NULL if the given file
    //   descriptor has not been added to this map.
    // No errors can occur.

    void put_unlocked(int fd, description *file) throw();
    // Effect: simply adds given description pointer to array, no locks acquired.
    description* get_unlocked(int fd) throw(); // use this one instead of get() when you already have the lock.
    int erase(int fd) throw() __attribute__((warn_unused_result)); // returns 0 or an error number.
    int size(void) throw();
private:
    void grow_array(int fd) throw();
    
friend class fmap_unit_test;
};

// Global locks used when the file descriptor map is updated.   Sometimes the backup system needs to hold the lock for several operations.
// If an error occurs, it's reported and the error number is returned.  If no error then returns 0.
void lock_fmap(void) throw();
void unlock_fmap(void) throw();

#endif // End of header guardian.