#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "storage_engine.h"
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include <memory>

namespace my_redis {

// Command response
struct CommandResponse {
    enum class Type {
        STRING,
        INTEGER,
        ARRAY,
        ERROR,
        NIL
    };
    
    Type type;
    std::string str_val;
    int64_t int_val;
    std::vector<CommandResponse> array_val;
    
    // Constructors for different types
    static CommandResponse create_string(const std::string& val);
    static CommandResponse create_integer(int64_t val);
    static CommandResponse create_array(const std::vector<CommandResponse>& val);
    static CommandResponse create_error(const std::string& val);
    static CommandResponse create_nil();
    
    // Serialize to RESP protocol
    std::string serialize() const;
};

// Command handler class
class CommandHandler {
public:
    using CommandFunc = std::function<CommandResponse(const std::vector<std::string>&, StorageEngine&)>;
    
    CommandHandler(StorageEngine& storage);
    ~CommandHandler() = default;
    
    // Process a command
    CommandResponse process_command(const std::vector<std::string>& command);
    
    // Parse a raw command string
    static std::vector<std::string> parse_command(const std::string& command_str);

private:
    StorageEngine& storage_;
    std::unordered_map<std::string, CommandFunc> command_map_;
    
    // Initialize command map
    void init_commands();
    
    // Command implementations
    static CommandResponse cmd_ping(const std::vector<std::string>& args, StorageEngine& storage);
    static CommandResponse cmd_set(const std::vector<std::string>& args, StorageEngine& storage);
    static CommandResponse cmd_get(const std::vector<std::string>& args, StorageEngine& storage);
    static CommandResponse cmd_del(const std::vector<std::string>& args, StorageEngine& storage);
    static CommandResponse cmd_exists(const std::vector<std::string>& args, StorageEngine& storage);
    static CommandResponse cmd_expire(const std::vector<std::string>& args, StorageEngine& storage);
    static CommandResponse cmd_ttl(const std::vector<std::string>& args, StorageEngine& storage);
    static CommandResponse cmd_dbsize(const std::vector<std::string>& args, StorageEngine& storage);
    static CommandResponse cmd_flushdb(const std::vector<std::string>& args, StorageEngine& storage);
    
    // Sorted set commands
    static CommandResponse cmd_zadd(const std::vector<std::string>& args, StorageEngine& storage);
    static CommandResponse cmd_zrem(const std::vector<std::string>& args, StorageEngine& storage);
    static CommandResponse cmd_zscore(const std::vector<std::string>& args, StorageEngine& storage);
    static CommandResponse cmd_zrange(const std::vector<std::string>& args, StorageEngine& storage);
    static CommandResponse cmd_zrangebyscore(const std::vector<std::string>& args, StorageEngine& storage);
    static CommandResponse cmd_zrank(const std::vector<std::string>& args, StorageEngine& storage);
    static CommandResponse cmd_zcard(const std::vector<std::string>& args, StorageEngine& storage);
};

} // namespace my_redis

#endif // COMMAND_HANDLER_H 