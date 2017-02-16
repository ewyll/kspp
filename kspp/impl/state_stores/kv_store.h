#pragma once
namespace kspp {
  template<class K, class V>
  class kv_store
  {
  public:
    //kv_store(boost::filesystem::path storage_path, std::shared_ptr<CODEC> codec) {}

    virtual ~kv_store() {}

    virtual void close() = 0;
    /**
    * Put a key-value pair
    */
    virtual void insert(std::shared_ptr<krecord<K, V>> record) = 0;
    /**
    * Deletes a key-value pair with the given key
    */
    //virtual void erase(const K& key) = 0;

    /**
    * commits the offset
    */
    virtual void commit() =0;

    // should be removed
    virtual void flush_offset() = 0;

    /** 
    * returns last offset
    */
    virtual int64_t offset() const = 0;

    virtual void start(int64_t offset) = 0;

    /**
    * Returns a key-value pair with the given key
    */
    virtual std::shared_ptr<krecord<K, V>> get(const K& key) = 0;
    virtual typename kspp::materialized_partition_source<K, V>::iterator begin(void) = 0;
    virtual typename kspp::materialized_partition_source<K, V>::iterator end() = 0;
  };
 };
