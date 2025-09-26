#!/usr/bin/env python3
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, OpaqueFunction
from launch.substitutions import LaunchConfiguration as LC
from launch_ros.actions import Node
from launch.actions import ExecuteProcess

def _build(context, *args, **kwargs):
    # Read launch args (as Python values)
    def _get(name, default=None):
        v = LC(name).perform(context)
        return v if v != '' else default

    use_cli                 = _get('use_cli') == 'true'
    all_topics              = _get('all') == 'true'
    topics_csv              = _get('topics', '')
    regex                   = _get('regex')
    exclude_regex           = _get('exclude_regex')
    output                  = _get('output', '')
    storage_id              = _get('storage_id', 'mcap')  # 'sqlite3' or 'mcap'
    compression_mode        = _get('compression_mode')    # 'file' or 'message'
    compression_format      = _get('compression_format')  # 'zstd' (Humble)
    include_hidden          = _get('include_hidden') == 'true'
    max_bag_size            = _get('max_bag_size')        # bytes
    max_bag_duration        = _get('max_bag_duration')    # seconds
    max_cache_size          = _get('max_cache_size')      # bytes
    storage_config_file     = _get('storage_config_file') # YAML for storage plugin
    qos_overrides_path      = _get('qos_overrides_path')  # YAML with QoS per-topic
    snapshot                = _get('snapshot') == 'true'
    polling_interval        = _get('polling_interval')    # ms
    use_filename_timestamp  = _get('use_filename_timestamp') == 'true'
    node_name               = _get('node_name', 'rosbag2_recorder')
    use_sim_time            = _get('use_sim_time') == 'true'

    # Build argument vector for rosbag2
    args = ['record']

    # Destination/output
    if output:
        args += ['-o', output]

    # Topic selection: topics list / -a / regex (mutually combinable as rosbag2 allows)
    if all_topics:
        args += ['-a']

    if regex:
        args += ['-e', regex]

    if exclude_regex:
        args += ['-x', exclude_regex]

    # Comma/space-separated topics → individual args
    if topics_csv:
        topics = [t.strip() for t in topics_csv.replace(',', ' ').split() if t.strip()]
        args += topics

    # Storage backend
    if storage_id:
        args += ['-s', storage_id]

    # Splitting / limits
    if max_bag_size:
        args += ['-b', max_bag_size]
    if max_bag_duration:
        args += ['-d', max_bag_duration]
    if max_cache_size:
        args += ['--max-cache-size', max_cache_size]

    # Hidden topics
    if include_hidden:
        args += ['--include-hidden-topics']

    # Compression
    if compression_mode:
        args += ['--compression-mode', compression_mode]
    if compression_format:
        args += ['--compression-format', compression_format]

    # Advanced
    if storage_config_file:
        args += ['-p', storage_config_file]
    if qos_overrides_path:
        args += ['--qos-profile-overrides-path', qos_overrides_path]
    if snapshot:
        args += ['--snapshot']
    if polling_interval:
        args += ['--polling-interval', polling_interval]
    if use_filename_timestamp:
        args += ['--use-filename-timestamp']

    # Choose Node vs CLI
    if use_cli:
        action = ExecuteProcess(
            cmd=['ros2', 'bag'] + args,
            output='screen'
        )
    else:
        # rosbag2_transport recorder node takes the same CLI args
        action = Node(
            package='rosbag2_transport',
            executable='record',
            name=node_name,
            output='screen',
            arguments=args[1:],  # drop the leading 'record'
            parameters=[{'use_sim_time': use_sim_time}]
        )

    return [action]

def generate_launch_description():
    return LaunchDescription([
        # Selection
        DeclareLaunchArgument('topics',              default_value='',     description='Comma/space-separated topic names to record'),
        DeclareLaunchArgument('all',                 default_value='false',description='Record all topics (-a)'),
        DeclareLaunchArgument('regex',               default_value='',     description='Regex to include topics (-e)'),
        DeclareLaunchArgument('exclude_regex',       default_value='',     description='Regex to exclude topics (-x)'),

        # Output / storage
        DeclareLaunchArgument('output',              default_value='',     description='Output directory or bag prefix (-o)'),
        DeclareLaunchArgument('storage_id',          default_value='mcap', description='Storage plugin: mcap or sqlite3 (-s)'),

        # Compression
        DeclareLaunchArgument('compression_mode',    default_value='',     description='Compression mode: file or message'),
        DeclareLaunchArgument('compression_format',  default_value='zstd', description='Compression format: zstd (if empty, no compression)'),

        # Limits & splitting
        DeclareLaunchArgument('max_bag_size',        default_value='',     description='Max bag size in bytes before split (-b)'),
        DeclareLaunchArgument('max_bag_duration',    default_value='',     description='Max duration in seconds before split (-d)'),
        DeclareLaunchArgument('max_cache_size',      default_value='',     description='Max recorder cache size in bytes'),

        # Advanced
        DeclareLaunchArgument('include_hidden',      default_value='false',description='Include hidden topics'),
        DeclareLaunchArgument('storage_config_file', default_value='',     description='YAML for storage plugin (-p)'),
        DeclareLaunchArgument('qos_overrides_path',  default_value='',     description='YAML for per-topic QoS overrides'),
        DeclareLaunchArgument('snapshot',            default_value='false',description='Snapshot mode (triggered writes)'),
        DeclareLaunchArgument('polling_interval',    default_value='',     description='Topic discovery polling interval (ms)'),
        DeclareLaunchArgument('use_filename_timestamp', default_value='false', description='Append start timestamp to bag name'),
        DeclareLaunchArgument('use_sim_time',        default_value='false',description='Set recorder node use_sim_time param'),
        DeclareLaunchArgument('node_name',           default_value='rosbag2_recorder', description='Name of the recorder node'),
        DeclareLaunchArgument('use_cli',             default_value='false',description='Use ExecuteProcess with "ros2 bag record" instead of the recorder node'),
        OpaqueFunction(function=_build),
    ])
