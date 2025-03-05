#!/usr/bin/env python3
"""
Test script for custom Redis-inspired key-value store.
This script tests all the core functionality of the server.
"""

import socket
import time
import random
import string
import threading
import subprocess
import os
import sys
import signal
import unittest
from typing import List, Tuple, Optional, Dict, Any

# Server configuration
SERVER_HOST = '127.0.0.1'
SERVER_PORT = 6380  # Using a non-standard port for testing
SERVER_EXECUTABLE = '../build/my_redis_server'

class RedisClient:
    """Simple Redis client for testing."""
    
    def __init__(self, host: str = SERVER_HOST, port: int = SERVER_PORT):
        self.host = host
        self.port = port
        self.socket = None
        
    def connect(self) -> bool:
        """Connect to the Redis server."""
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.connect((self.host, self.port))
            return True
        except Exception as e:
            print(f"Failed to connect: {e}")
            return False
            
    def disconnect(self) -> None:
        """Disconnect from the Redis server."""
        if self.socket:
            self.socket.close()
            self.socket = None
            
    def send_command(self, command: str) -> str:
        """Send a command to the Redis server and return the response."""
        if not self.socket:
            raise RuntimeError("Not connected to server")
            
        # Send command
        self.socket.sendall(f"{command}\r\n".encode())
        
        # Receive response
        response = self.socket.recv(4096).decode()
        return response.strip()
        
    # Basic commands
    def ping(self, message: Optional[str] = None) -> str:
        """Send a PING command."""
        if message:
            return self.send_command(f"PING {message}")
        return self.send_command("PING")
        
    def set(self, key: str, value: str) -> str:
        """Set a key-value pair."""
        return self.send_command(f"SET {key} {value}")
        
    def set_ex(self, key: str, value: str, seconds: int) -> str:
        """Set a key-value pair with expiration."""
        return self.send_command(f"SET {key} {value} EX {seconds}")
        
    def set_nx(self, key: str, value: str) -> str:
        """Set a key-value pair if the key does not exist."""
        return self.send_command(f"SET {key} {value} NX")
        
    def get(self, key: str) -> str:
        """Get the value of a key."""
        return self.send_command(f"GET {key}")
        
    def delete(self, *keys: str) -> str:
        """Delete one or more keys."""
        return self.send_command(f"DEL {' '.join(keys)}")
        
    def exists(self, *keys: str) -> str:
        """Check if one or more keys exist."""
        return self.send_command(f"EXISTS {' '.join(keys)}")
        
    def expire(self, key: str, seconds: int) -> str:
        """Set a key's time to live in seconds."""
        return self.send_command(f"EXPIRE {key} {seconds}")
        
    def ttl(self, key: str) -> str:
        """Get the time to live for a key in seconds."""
        return self.send_command(f"TTL {key}")
        
    def dbsize(self) -> str:
        """Get the number of keys in the database."""
        return self.send_command("DBSIZE")
        
    def flushdb(self) -> str:
        """Remove all keys from the database."""
        return self.send_command("FLUSHDB")
        
    # Sorted set commands
    def zadd(self, key: str, *score_members: Tuple[float, str]) -> str:
        """Add members to a sorted set."""
        args = []
        for score, member in score_members:
            args.extend([str(score), member])
        return self.send_command(f"ZADD {key} {' '.join(args)}")
        
    def zrem(self, key: str, *members: str) -> str:
        """Remove members from a sorted set."""
        return self.send_command(f"ZREM {key} {' '.join(members)}")
        
    def zscore(self, key: str, member: str) -> str:
        """Get the score of a member in a sorted set."""
        return self.send_command(f"ZSCORE {key} {member}")
        
    def zrange(self, key: str, start: int, stop: int, with_scores: bool = False) -> str:
        """Get a range of members in a sorted set by index."""
        cmd = f"ZRANGE {key} {start} {stop}"
        if with_scores:
            cmd += " WITHSCORES"
        return self.send_command(cmd)
        
    def zrangebyscore(self, key: str, min_score: float, max_score: float, with_scores: bool = False) -> str:
        """Get a range of members in a sorted set by score."""
        cmd = f"ZRANGEBYSCORE {key} {min_score} {max_score}"
        if with_scores:
            cmd += " WITHSCORES"
        return self.send_command(cmd)
        
    def zrank(self, key: str, member: str) -> str:
        """Get the rank of a member in a sorted set."""
        return self.send_command(f"ZRANK {key} {member}")
        
    def zcard(self, key: str) -> str:
        """Get the number of members in a sorted set."""
        return self.send_command(f"ZCARD {key}")


class ServerProcess:
    """Manage the Redis server process."""
    
    def __init__(self, executable: str = SERVER_EXECUTABLE, port: int = SERVER_PORT):
        self.executable = executable
        self.port = port
        self.process = None
        
    def start(self) -> bool:
        """Start the Redis server."""
        try:
            # Check if the executable exists
            if not os.path.exists(self.executable):
                print(f"Server executable not found: {self.executable}")
                return False
                
            # Start the server process
            self.process = subprocess.Popen(
                [self.executable, str(self.port)],
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE
            )
            
            # Wait for the server to start
            time.sleep(1)
            
            # Check if the process is still running
            if self.process.poll() is not None:
                stdout, stderr = self.process.communicate()
                print(f"Server failed to start: {stderr.decode()}")
                return False
                
            return True
        except Exception as e:
            print(f"Failed to start server: {e}")
            return False
            
    def stop(self) -> None:
        """Stop the Redis server."""
        if self.process:
            try:
                self.process.terminate()
                self.process.wait(timeout=5)
            except subprocess.TimeoutExpired:
                print("Server did not terminate within timeout, forcing kill...")
                self.process.kill()
                self.process.wait()
            finally:
                self.process = None


class RedisTest(unittest.TestCase):
    """Test cases for the Redis server."""
    
    @classmethod
    def setUpClass(cls):
        """Set up the test environment."""
        cls.server = ServerProcess()
        if not cls.server.start():
            # Try to build the server if it doesn't exist
            print("Attempting to build the server...")
            build_dir = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "build")
            if not os.path.exists(build_dir):
                os.makedirs(build_dir)
            os.chdir(build_dir)
            subprocess.run(["cmake", ".."])
            subprocess.run(["make"])
            os.chdir(os.path.dirname(os.path.abspath(__file__)))
            
            # Try again
            if not cls.server.start():
                print("Failed to start server. Please build it manually.")
                sys.exit(1)
                
        cls.client = RedisClient()
        if not cls.client.connect():
            cls.server.stop()
            print("Failed to connect to server.")
            sys.exit(1)
            
    @classmethod
    def tearDownClass(cls):
        """Clean up the test environment."""
        cls.client.disconnect()
        cls.server.stop()
        
    def setUp(self):
        """Set up each test."""
        self.client.flushdb()
        
    def test_ping(self):
        """Test the PING command."""
        self.assertEqual(self.client.ping(), "+PONG")
        self.assertEqual(self.client.ping("hello"), "+hello")
        
    def test_set_get(self):
        """Test the SET and GET commands."""
        self.assertEqual(self.client.set("key1", "value1"), "+OK")
        self.assertEqual(self.client.get("key1"), "+value1")
        self.assertEqual(self.client.get("nonexistent"), "$-1")
        
    def test_set_ex(self):
        """Test the SET command with expiration."""
        self.assertEqual(self.client.set_ex("key1", "value1", 1), "+OK")
        self.assertEqual(self.client.get("key1"), "+value1")
        time.sleep(2)
        self.assertEqual(self.client.get("key1"), "$-1")
        
    def test_set_nx(self):
        """Test the SET command with NX option."""
        self.assertEqual(self.client.set_nx("key1", "value1"), "+OK")
        self.assertEqual(self.client.set_nx("key1", "value2"), "$-1")
        self.assertEqual(self.client.get("key1"), "+value1")
        
    def test_del(self):
        """Test the DEL command."""
        self.client.set("key1", "value1")
        self.client.set("key2", "value2")
        self.assertEqual(self.client.delete("key1", "key2", "nonexistent"), ":2")
        self.assertEqual(self.client.get("key1"), "$-1")
        self.assertEqual(self.client.get("key2"), "$-1")
        
    def test_exists(self):
        """Test the EXISTS command."""
        self.client.set("key1", "value1")
        self.client.set("key2", "value2")
        self.assertEqual(self.client.exists("key1", "key2", "nonexistent"), ":2")
        
    def test_expire_ttl(self):
        """Test the EXPIRE and TTL commands."""
        self.client.set("key1", "value1")
        self.assertEqual(self.client.expire("key1", 5), ":1")
        ttl = int(self.client.ttl("key1").strip(":"))
        self.assertTrue(0 <= ttl <= 5)
        self.assertEqual(self.client.ttl("nonexistent"), ":-2")
        
    def test_dbsize(self):
        """Test the DBSIZE command."""
        self.assertEqual(self.client.dbsize(), ":0")
        self.client.set("key1", "value1")
        self.client.set("key2", "value2")
        self.assertEqual(self.client.dbsize(), ":2")
        
    def test_flushdb(self):
        """Test the FLUSHDB command."""
        self.client.set("key1", "value1")
        self.client.set("key2", "value2")
        self.assertEqual(self.client.flushdb(), "+OK")
        self.assertEqual(self.client.dbsize(), ":0")
        
    def test_zadd_zcard(self):
        """Test the ZADD and ZCARD commands."""
        self.assertEqual(self.client.zadd("zset1", (1.0, "one"), (2.0, "two"), (3.0, "three")), ":3")
        self.assertEqual(self.client.zcard("zset1"), ":3")
        self.assertEqual(self.client.zadd("zset1", (4.0, "four"), (5.0, "five")), ":2")
        self.assertEqual(self.client.zcard("zset1"), ":5")
        
    def test_zscore(self):
        """Test the ZSCORE command."""
        self.client.zadd("zset1", (1.0, "one"), (2.0, "two"), (3.0, "three"))
        self.assertEqual(self.client.zscore("zset1", "one"), "+1.000000")
        self.assertEqual(self.client.zscore("zset1", "nonexistent"), "$-1")
        
    def test_zrange(self):
        """Test the ZRANGE command."""
        self.client.zadd("zset1", (1.0, "one"), (2.0, "two"), (3.0, "three"))
        self.assertEqual(self.client.zrange("zset1", 0, 1), "*2\r\n+one\r\n+two")
        self.assertEqual(self.client.zrange("zset1", 0, -1), "*3\r\n+one\r\n+two\r\n+three")
        self.assertEqual(self.client.zrange("zset1", 0, 1, with_scores=True), 
                         "*4\r\n+one\r\n+1.000000\r\n+two\r\n+2.000000")
        
    def test_zrangebyscore(self):
        """Test the ZRANGEBYSCORE command."""
        self.client.zadd("zset1", (1.0, "one"), (2.0, "two"), (3.0, "three"))
        self.assertEqual(self.client.zrangebyscore("zset1", 1.0, 2.0), "*2\r\n+one\r\n+two")
        self.assertEqual(self.client.zrangebyscore("zset1", 1.0, 3.0, with_scores=True), 
                         "*6\r\n+one\r\n+1.000000\r\n+two\r\n+2.000000\r\n+three\r\n+3.000000")
        
    def test_zrank(self):
        """Test the ZRANK command."""
        self.client.zadd("zset1", (1.0, "one"), (2.0, "two"), (3.0, "three"))
        self.assertEqual(self.client.zrank("zset1", "one"), ":0")
        self.assertEqual(self.client.zrank("zset1", "three"), ":2")
        self.assertEqual(self.client.zrank("zset1", "nonexistent"), "$-1")
        
    def test_zrem(self):
        """Test the ZREM command."""
        self.client.zadd("zset1", (1.0, "one"), (2.0, "two"), (3.0, "three"))
        self.assertEqual(self.client.zrem("zset1", "two"), ":1")
        self.assertEqual(self.client.zcard("zset1"), ":2")
        self.assertEqual(self.client.zrange("zset1", 0, -1), "*2\r\n+one\r\n+three")
        
    def test_concurrent_operations(self):
        """Test concurrent operations."""
        def worker(client, prefix, count):
            client.connect()
            for i in range(count):
                key = f"{prefix}:{i}"
                client.set(key, f"value:{i}")
                time.sleep(0.01)
            client.disconnect()
            
        # Create multiple clients
        clients = [RedisClient() for _ in range(5)]
        threads = []
        
        # Start concurrent operations
        for i, client in enumerate(clients):
            thread = threading.Thread(target=worker, args=(client, f"client{i}", 20))
            threads.append(thread)
            thread.start()
            
        # Wait for all threads to complete
        for thread in threads:
            thread.join()
            
        # Verify results
        self.assertEqual(int(self.client.dbsize().strip(":")), 100)
        
    def test_large_dataset(self):
        """Test with a large dataset."""
        # Generate random keys and values
        keys = [f"key:{i}" for i in range(1000)]
        values = [''.join(random.choices(string.ascii_letters, k=10)) for _ in range(1000)]
        
        # Set all keys
        for key, value in zip(keys, values):
            self.client.set(key, value)
            
        # Verify all keys
        for key, value in zip(keys, values):
            self.assertEqual(self.client.get(key), f"+{value}")
            
        # Delete all keys
        for i in range(0, 1000, 10):
            batch = keys[i:i+10]
            self.client.delete(*batch)
            
        # Verify all keys are deleted
        self.assertEqual(self.client.dbsize(), ":0")
        
    def test_ttl_expiration(self):
        """Test TTL expiration with many keys."""
        # Set keys with different TTLs
        for i in range(10):
            self.client.set_ex(f"key:{i}", f"value:{i}", i + 1)
            
        # Wait for all keys to expire
        time.sleep(11)
        
        # Verify all keys are expired
        self.assertEqual(self.client.dbsize(), ":0")
        
    def test_sorted_set_operations(self):
        """Test complex sorted set operations."""
        # Add members to a sorted set
        members = [(float(i), f"member:{i}") for i in range(100)]
        self.client.zadd("zset1", *members)
        
        # Test range queries
        self.assertEqual(self.client.zcard("zset1"), ":100")
        
        # Test score updates
        for i in range(0, 100, 2):
            self.client.zadd("zset1", (float(i + 100), f"member:{i}"))
            
        # Verify score updates
        for i in range(0, 100, 2):
            self.assertEqual(self.client.zscore("zset1", f"member:{i}"), f"+{float(i + 100)}")
            
        # Test range by score
        result = self.client.zrangebyscore("zset1", 100, 110)
        self.assertTrue("+member:0" in result)
        self.assertTrue("+member:2" in result)
        self.assertTrue("+member:4" in result)
        self.assertTrue("+member:6" in result)
        self.assertTrue("+member:8" in result)
        self.assertTrue("+member:10" in result)


if __name__ == "__main__":
    unittest.main() 