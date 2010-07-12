require 'pg'
require 'etc'

class Benchmarks
  class Pg
    class Statement
      attr_reader :rows
      def initialize conn, name
        @conn, @name = conn, name
        @rows, @cols, @row = 0, 0 ,0
      end
      def execute *args
        @res  = @conn.exec_prepared @name, args
        @rows = @res.ntuples
        @cols = @res.nfields
      end
      def fetchrow
        if @row < @rows
          @row += 1
          @cols.times.map {|c| @res.getisnull(@row-1,c) ? nil : @res.getvalue(@row-1, c) }
        else
          nil
        end
      end
      def clear
        @row = 0
        @res.clear
      end
    end

    class Connection < PGconn
      def prepare name, sql
        rs = super
        Statement.new(self, name)
      end
    end

    attr_reader :driver

    def initialize driver, sql
      @driver, @sql = driver, process_command(sql)
      @h = Connection.connect "host=127.0.0.1 dbname=dbicpp user=#{Etc.getlogin}"
      @sth = @h.prepare "qbench", @sql
    end

    def process_command sql
      n = 1
      while sql.match(/(?<!')\?(?!')/)
        sql.sub!(/(?<!')\?(?!')/, "$#{n}")
        n += 1
      end
      sql
    end

    def run n, *args
      open("/dev/null", "w") do |fh|
        n.times do
          @sth.execute(*args)
          while r = @sth.fetchrow
            fh.puts "#{r[0]} #{r[1]} #{r[2]}"
          end
          @sth.clear
        end
      end
    end

    def self.name
      "ruby: libpq"
    end
  end

  register :postgresql, Pg
end
